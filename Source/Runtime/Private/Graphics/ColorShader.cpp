/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/ColorShader.h"

FColorShader::FColorShader() : FShader()
{
	VertexShaderFile = L"C:/MadnessEngine/MadnessEngine/Source/Shaders/Color.vs";
	PixelShaderFile = L"C:/MadnessEngine/MadnessEngine/Source/Shaders/Color.ps";
	VertexFunctionName = "ColorVertexShader";
	PixelFunctionName = "ColorPixelShader";
}

FColorShader::FColorShader(const FColorShader &Other) : FShader(Other)
{

}

FColorShader::~FColorShader()
{
	
}