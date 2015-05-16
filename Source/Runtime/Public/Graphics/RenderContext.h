/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
#include "Runtime/Public/Graphics/D3DHandle.h"
#endif

#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"

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

private:
	/* Renders a single frame. */
	bool Render();

#if DIRECTX
	/* This handles interaction with DirectX. */
	FD3DHandle* DirectXHandle;

	/* Camera used to render the scene from. */
	FCamera* Camera;

	/* The model (triangle) we're rendering. */
	FModel* Model;

	/* The shader used to render the model. */
	FColorShader* ColorShader;
#endif
};