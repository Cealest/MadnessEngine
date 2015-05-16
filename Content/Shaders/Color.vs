// Copyright Myles Salholm 2015

// Simple Color Vertex Shader

// Globals
cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

// Type definitions
struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// Vertex Shader
PixelInputType ColorVertexShader(VertexInputType Input)
{
	PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
	Input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(Input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	// Store the input color for the pixel shader to use.
	output.color = Input.color;

	return output;
}