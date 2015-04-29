/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
/* This is the first place we actually include the DirectX libraries. */
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

/*
Handles interactions with DirectX.
*/
class FD3DHandle
{
public:
	/* Constructor. */
	FD3DHandle();
	/* Copy Constructor. */
	FD3DHandle(const FD3DHandle& Other);
	/* Destructor. */
	~FD3DHandle();

	/* Sets up DirectX for rendering to the window. */
	bool Initialize(int ScreenWidth, int ScreenHeight, bool VSync, HWND WindowHandle, float ScreenDepth, float ScreenNear);

	/* Shuts down DirectX. */
	void Shutdown();

	/* Clears the back buffer to the passed in color. */
	void BeginScene(float Red, float Green, float Blue, float Alpha);

	/* Presents the back buffer to the screen. */
	void EndScene();

	/* Returns reference to our DirectX 11 Device. */
	ID3D11Device* GetDevice();

	/* Returns reference to our DirectX 11 Device Context. */
	ID3D11DeviceContext* GetDeviceContext();

	/* Sets OutD3DMatrix to the projection matrix. */
	void GetProjectionMatrix(D3DXMATRIX& OutD3DMatrix);
	/* Sets OutD3DMatrix to the world matrix. */
	void GetWorldMatrix(D3DXMATRIX& OutD3DMatrix);
	/* Sets OutD3DMatrix to the orthographic matrix. */
	void GetOrthoMatrix(D3DXMATRIX& OutD3DMatrix);

	/* Sets OutString to the graphics card info and OutMemory to the available memory. */
	void GetVideoCardInfo(char* OutString, int& OutMemory);

private:
	/* Is actively using vertical sync. */
	bool VSyncEnabled;

	/* How much memory we get. */
	int VideoCardMemory;

	/* Some extra card info. */
	char VideoCardDescription[128];

	/* DirectX SwapChain. */
	IDXGISwapChain* SwapChain;

	/* Graphics Device. */
	ID3D11Device* Device;

	/* Graphics Device Context. */
	ID3D11DeviceContext* DeviceContext;

	/* Render target view. */
	ID3D11RenderTargetView* RenderTargetView;

	/* Buffer for depth shading. */
	ID3D11Texture2D* DepthStencilBuffer;

	/* Current state for depth shading. */
	ID3D11DepthStencilState* DepthStencilState;

	/* View for depth shading. */
	ID3D11DepthStencilView* DepthStencilView;

	/* The rasterizer. */
	ID3D11RasterizerState* RasterState;

	/* Projection Matrix. */
	D3DXMATRIX ProjectionMatrix;

	/* World Matrix. */
	D3DXMATRIX WorldMatrix;

	/* Orthographic Matrix. */
	D3DXMATRIX OrthoMatrix;

};
#endif