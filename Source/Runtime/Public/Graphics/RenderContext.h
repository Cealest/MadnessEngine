/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
#include "Runtime/Public/Graphics/D3DHandle.h"
#endif

/* Global variables free for all. */
const bool GFullScreen = false;
const bool GVerticalSync = true;
const float GFarClip = 100000.0f;
const float GNearClip = 0.001f;

/* 
The context which determines how the rendering engine behaves.
*/
class FRenderContext
{
public:
	/* Constructor. */
	FRenderContext();
	/* Destructor. */
	~FRenderContext();

	/* Initialized the render context. */
	bool Init(class FWindow* Owner);

	/* Shuts down the render context. */
	void Shutdown();

	/* Processes a single frame. */
	bool Frame();

	/* The window that owns this render context. */
	class FWindow* WindowOwner;

	/* Sets the type of shader to render. */
	void SetShaderType(EShader::Type InShaderType);
	/* Returns the current type of shader to render. */
	const EShader::Type GetShaderType() const;

private:
	/* Renders a single frame. */
	bool Render(float InRotation /* only for prototyping, remove argument later */);

	/* What type of shader we're rendering. */
	EShader::Type ShaderType;

#if DIRECTX
	/* This handles interaction with DirectX. */
	FD3DHandle* DirectXHandle;

	/* Camera used to render the scene from. */
	class FCamera* Camera;

	/* The model (triangle) we're rendering. */
	class FModel* Model;

	/* The shader used to render the model. */
	class FColorShader* ColorShader;

	/* The shader used to render the model. */
	class FTextureShader* TextureShader;

	/* The shader used to render the model. */
	class FLightShader* LightShader;

	/* The directional light in the scene. */
	class FLight* Light;
#endif
};