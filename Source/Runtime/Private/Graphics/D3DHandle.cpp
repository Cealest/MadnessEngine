/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/D3DHandle.h"
#include "Runtime/Public/Graphics/RenderContext.h"

#if DIRECTX

FD3DHandle::FD3DHandle()
{
	VSyncEnabled = false;
	VideoCardMemory = 0;
	SwapChain = nullptr;
	Device = nullptr;
	DeviceContext = nullptr;
	RenderTargetView = nullptr;
	DepthStencilBuffer = nullptr;
	DepthStencilState = nullptr;
	DepthStencilView = nullptr;
	RasterState = nullptr;
}

FD3DHandle::FD3DHandle(const FD3DHandle& Other)
{

}

FD3DHandle::~FD3DHandle()
{

}

bool FD3DHandle::Initialize(int ScreenWidth, int ScreenHeight, bool VSync, HWND WindowHandle, float ScreenDepth, float ScreenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// Store vertical sync
	VSyncEnabled = VSync;

	// Create the DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the video card.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output, or monitor.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit our display format for the monitor.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold possible display modes for this monitor and video card combo.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (displayModeList == nullptr)
	{
		return false;
	}

	// Fill the display mode list struct.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Find a display mode that matches the screen width and height, then store its refresh rate.
	for (i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)ScreenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)ScreenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the video card description
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the video card memory in MBs.
	VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a char array and store it.
	error = wcstombs_s(&stringLength, VideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = nullptr;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = nullptr;

	// Release the adapter.
	adapter->Release();
	adapter = nullptr;

	// Release the factory.
	factory->Release();
	factory = nullptr;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Just one back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the dimension of the back buffer.
	swapChainDesc.BufferDesc.Width = ScreenWidth;
	swapChainDesc.BufferDesc.Height = ScreenHeight;

	// Set it to our format.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (VSyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		// Just render as fast as possible.
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the back buffer's usage.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the window handle to render to.
	swapChainDesc.OutputWindow = WindowHandle;

	//@TODO Make multisampling configurable.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (GFullScreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Unspecified scan line order and scaling
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Clear the back buffer after swapping.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// No advanced flags for now
	swapChainDesc.Flags = 0;

	// Which DirectX Version to use.
	//@TODO Support DX9 & DX10
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, D3D device, and D3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &Device, NULL, &DeviceContext);
	if (FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = Device->CreateRenderTargetView(backBufferPtr, NULL, &RenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release the pointer to the back buffer.
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	// Initialize our depth buffer description.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Flesh out the description of the depth buffer.
	depthBufferDesc.Width = ScreenWidth;
	depthBufferDesc.Height = ScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer with that description
	result = Device->CreateTexture2D(&depthBufferDesc, NULL, &DepthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize our depth stencil description.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Flesh out the description of the depth stencil.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil with that description.
	result = Device->CreateDepthStencilState(&depthStencilDesc, &DepthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	DeviceContext->OMSetDepthStencilState(DepthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = Device->CreateDepthStencilView(DepthStencilBuffer, &depthStencilViewDesc, &DepthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	// Setup the rasterizer description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state with our description.
	result = Device->CreateRasterizerState(&rasterDesc, &RasterState);
	if (FAILED(result))
	{
		return false;
	}

	// Set the rasterizer state.
	DeviceContext->RSSetState(RasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)ScreenWidth;
	viewport.Height = (float)ScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	DeviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)ScreenWidth / (float)ScreenHeight;

	// Create the projection matrix.
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, fieldOfView, screenAspect, ScreenNear, ScreenDepth);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&WorldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&OrthoMatrix, (float)ScreenWidth, (float)ScreenHeight, ScreenNear, ScreenDepth);

	return true;
}

void FD3DHandle::Shutdown()
{
	// Before shutting down, switch to windowed mode or when you release the swap chain it'll throw an exception.
	if (SwapChain)
	{
		SwapChain->SetFullscreenState(false, NULL);
	}

	if (RasterState)
	{
		RasterState->Release();
		RasterState = nullptr;
	}

	if (DepthStencilView)
	{
		DepthStencilView->Release();
		DepthStencilView = nullptr;
	}

	if (DepthStencilState)
	{
		DepthStencilState->Release();
		DepthStencilState = nullptr;
	}

	if (DepthStencilBuffer)
	{
		DepthStencilBuffer->Release();
		DepthStencilBuffer = nullptr;
	}

	if (RenderTargetView)
	{
		RenderTargetView->Release();
		RenderTargetView = nullptr;
	}

	if (DeviceContext)
	{
		DeviceContext->Release();
		DeviceContext = nullptr;
	}

	if (Device)
	{
		Device->Release();
		Device = nullptr;
	}

	if (SwapChain)
	{
		SwapChain->Release();
		SwapChain = nullptr;
	}
}

void FD3DHandle::BeginScene(float Red, float Green, float Blue, float Alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = Red;
	color[1] = Green;
	color[2] = Blue;
	color[3] = Alpha;

	// Clear the back buffer.
	DeviceContext->ClearRenderTargetView(RenderTargetView, color);

	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void FD3DHandle::EndScene()
{
	// Swap the back buffer to the screen
	if (VSyncEnabled)
	{
		// Lock to screen refresh rate.
		SwapChain->Present(1, 0);
	}
	else
	{
		// Show me now!
		SwapChain->Present(0, 0);
	}
}

ID3D11Device* FD3DHandle::GetDevice()
{
	return Device;
}

ID3D11DeviceContext* FD3DHandle::GetDeviceContext()
{
	return DeviceContext;
}

void FD3DHandle::GetProjectionMatrix(D3DXMATRIX& OutD3DMatrix)
{
	OutD3DMatrix = ProjectionMatrix;
}

void FD3DHandle::GetWorldMatrix(D3DXMATRIX& OutD3DMatrix)
{
	OutD3DMatrix = WorldMatrix;
}

void FD3DHandle::GetOrthoMatrix(D3DXMATRIX& OutD3DMatrix)
{
	OutD3DMatrix = OrthoMatrix;
}

void FD3DHandle::GetVideoCardInfo(char* OutString, int& OutMemory)
{
	strcpy_s(OutString, 128, VideoCardDescription);
	OutMemory = VideoCardMemory;
}
#endif