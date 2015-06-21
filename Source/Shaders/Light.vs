// Copyright Myles Salholm 2015

// Simple Lit Texture Vertex Shader

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
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// Vertex Shader
PixelInputType LightVertexShader(VertexInputType Input)
{
	PixelInputType output;

	// Change the position vector to be 4 units for proper matrix calculations.
	Input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(Input.position, WorldMatrix);
	output.position = mul(output.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	// Store the texture coordinate for the pixel shader.
	output.tex = Input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(Input.normal, (float3x3)WorldMatrix);

	// Normalize the vector
	output.normal = normalize(output.normal);

	return output;
}