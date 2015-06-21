/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/RenderContext.h"
#include "Runtime/Public/Core/Window.h"
#include "Runtime/Public/Core/ProgramInstance.h"
#include "Runtime/Public/Graphics/Camera.h"
#include "Runtime/Public/Graphics/Model.h"
#include "Runtime/Public/Graphics/ColorShader.h"
#include "Runtime/Public/Graphics/TextureShader.h"
#include "Runtime/Public/Graphics/LightShader.h"
#include "Runtime/Public/Graphics/Light.h"

FRenderContext::FRenderContext()
{
	WindowOwner = nullptr;

#if DIRECTX
	DirectXHandle = nullptr;
	Camera = nullptr;
	Model = nullptr;
	ColorShader = nullptr;
	TextureShader = nullptr;
	LightShader = nullptr;
	ShaderType = EShader::DiffuseLight;
	Light = nullptr;
#endif
}

FRenderContext::~FRenderContext()
{
#if DIRECTX
	if (Light)
	{
		delete Light;
	}

	if (LightShader)
	{
		LightShader->Shutdown();
		delete LightShader;
	}

	if (ColorShader)
	{
		ColorShader->Shutdown();
		delete ColorShader;
	}
	
	if (TextureShader)
	{
		TextureShader->Shutdown();
		delete TextureShader;
	}

	if (Model)
	{
		Model->Shutdown();
		delete Model;
	}

	if (Camera)
	{
		delete Camera;
	}

	if (DirectXHandle)
	{
		DirectXHandle->Shutdown();
		delete DirectXHandle;
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
	if (!Model->Initialize(DirectXHandle->GetDevice(), "C:/MadnessEngine/MadnessEngine/Content/Meshes/Cube.txt", L"C:/MadnessEngine/MadnessEngine/Content/Textures/Test.dds"))
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

	// Create the texture shader
	TextureShader = new FTextureShader();
	if (!TextureShader)
	{
		return false;
	}
	// Initialize the shader
	if (!TextureShader->Initialize(DirectXHandle->GetDevice(), WindowOwner->WindowHandle, Model->GetTexture()))
	{
		MessageBox(WindowOwner->WindowHandle, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the lit texture shader
	LightShader = new FLightShader();
	if (!LightShader)
	{
		return false;
	}
	// Initializes the shader
	if (!LightShader->Initialize(DirectXHandle->GetDevice(), WindowOwner->WindowHandle, Model->GetTexture()))
	{
		MessageBox(WindowOwner->WindowHandle, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object
	Light = new FLight();
	if (!Light)
	{
		return false;
	}
	// Initialize the light object
	Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	Light->SetDirection(0.0f, 0.0f, 1.0f);
#endif

	return true;
}

void FRenderContext::Shutdown()
{
#if DIRECTX
	if (Light)
	{
		delete Light;
		Light = nullptr;
	}

	if (LightShader)
	{
		LightShader->Shutdown();
		delete LightShader;
		LightShader = nullptr;
	}

	if (ColorShader)
	{
		ColorShader->Shutdown();
		delete ColorShader;
		ColorShader = nullptr;
	}

	if (TextureShader)
	{
		TextureShader->Shutdown();
		delete TextureShader;
		TextureShader = nullptr;
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
	static float rotation = 0.0f;
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	bool result = Render(rotation);
	if (!result)
	{
		// Failed to render.
		return false;
	}

	return true;
}

bool FRenderContext::Render(float InRotation)
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

		// Rotate the world matrix by the rotation value so that the triangle will spin.
		D3DXMatrixRotationY(&worldMatrix, InRotation);

		// Put the model vertex and index buffers on the graphics pipeline to prepare for drawing.
		Model->Render(DirectXHandle->GetDeviceContext());

		switch (ShaderType)
		{
		case EShader::DiffuseLight:
			// Render the model using the lit texture shader.
			if (!LightShader->Render(DirectXHandle->GetDeviceContext(), Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, Light))
			{
				return false;
			}
			break;
		case EShader::Texture:
			// Render the model using the texture shader.
			if (!TextureShader->Render(DirectXHandle->GetDeviceContext(), Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
			{
				return false;
			}
			break;
		case EShader::Color:
			// Render the model using the color shader.
			if (!ColorShader->Render(DirectXHandle->GetDeviceContext(), Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
			{
				return false;
			}
			break;
		default:
			// Shader type unspecified.
			return true;
		}

		// Swap the buffers.
		DirectXHandle->EndScene();
	}
#endif
	return true;
}

void FRenderContext::SetShaderType(EShader::Type InShaderType)
{
	ShaderType = InShaderType;
}

const EShader::Type FRenderContext::GetShaderType() const
{
	return ShaderType;
}