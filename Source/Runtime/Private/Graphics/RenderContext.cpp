/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/RenderContext.h"
#include "Runtime/Public/Core/Window.h"
#include "Runtime/Public/Core/ProgramInstance.h"

FRenderContext::FRenderContext()
{
	WindowOwner = nullptr;

#if DIRECTX
	DirectXHandle = nullptr;
	Camera = nullptr;
	Model = nullptr;
	ColorShader = nullptr;
#endif
}

FRenderContext::~FRenderContext()
{
#if DIRECTX
	if (ColorShader)
	{
		ColorShader->Shutdown();
		delete ColorShader;
		ColorShader = nullptr;
	}

	if (Model)
	{
		Model->Shutdown();
		delete Model;
		Model = nullptr;
	}

	if (Camera)
	{
		delete Camera;
		Camera = nullptr;
	}

	if (DirectXHandle)
	{
		DirectXHandle->Shutdown();
		delete DirectXHandle;
		DirectXHandle = nullptr;
	}
#endif

	WindowOwner = nullptr;
}

bool FRenderContext::Init(FWindow* Owner)
{
	WindowOwner = Owner;
	if (!WindowOwner)
	{
		// No window owner, we don't know where to render to.
		return false;
	}

#if DIRECTX
	DirectXHandle = new FD3DHandle();
	if (!DirectXHandle)
	{
		// No DirectX interface and we're using DirectX.
		return false;
	}
	if (!DirectXHandle->Initialize(GProgramInstance.DefaultWidth, GProgramInstance.DefaultHeight, GVerticalSync, Owner->WindowHandle, GFarClip, GNearClip))
	{
		// Failed to initialize DirectX
		return false;
	}

	// Create the camera
	Camera = new FCamera();
	if (!Camera)
	{
		return false;
	}
	Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model
	Model = new FModel();
	if (!Model)
	{
		return false;
	}
	// Initialize the model
	if (!Model->Initialize(DirectXHandle->GetDevice()))
	{
		MessageBox(WindowOwner->WindowHandle, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader
	ColorShader = new FColorShader();
	if (!ColorShader)
	{
		return false;
	}
	// Initialize the shader
	if (!ColorShader->Initialize(DirectXHandle->GetDevice(), WindowOwner->WindowHandle))
	{
		MessageBox(WindowOwner->WindowHandle, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
#endif

	return true;
}

void FRenderContext::Shutdown()
{
#if DIRECTX
	if (ColorShader)
	{
		ColorShader->Shutdown();
		delete ColorShader;
		ColorShader = nullptr;
	}

	if (Model)
	{
		Model->Shutdown();
		delete Model;
		Model = nullptr;
	}

	if (Camera)
	{
		delete Camera;
		Camera = nullptr;
	}

	if (DirectXHandle)
	{
		DirectXHandle->Shutdown();
		delete DirectXHandle;
		DirectXHandle = nullptr;
	}
#endif
}

bool FRenderContext::Frame()
{
	//@TODO Re-evaluate whether this function is really necessary.

	bool result = Render();
	if (!result)
	{
		// Failed to render.
		return false;
	}

	return true;
}

bool FRenderContext::Render()
{
#if DIRECTX
	if (DirectXHandle)
	{
		D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;

		// Clear the buffers.
		DirectXHandle->BeginScene(0.2f, 0.2f, 0.2f, 1.0f);

		// Generate the view matrix with the camera.
		Camera->Render();

		// Get the world, view, and projection matrices
		Camera->GetViewMatrix(viewMatrix);
		DirectXHandle->GetWorldMatrix(worldMatrix);
		DirectXHandle->GetProjectionMatrix(projectionMatrix);

		// Put the model vertex and index buffers on the graphics pipeline to prepare for drawing.
		Model->Render(DirectXHandle->GetDeviceContext());

		// Render the model using the color shader.
		if (!ColorShader->Render(DirectXHandle->GetDeviceContext(), Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
		{
			return false;
		}

		// Swap the buffers.
		DirectXHandle->EndScene();
	}
#endif
	return true;
}