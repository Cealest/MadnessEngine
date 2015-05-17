/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#include "Runtime/Public/Graphics/Shader.h"

// A simple color shader
class FColorShader : public FShader
{
public:
	FColorShader();
	FColorShader(const FColorShader &Other);
	~FColorShader();
};