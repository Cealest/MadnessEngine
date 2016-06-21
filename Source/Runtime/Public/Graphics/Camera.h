/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if DIRECTX
#include <d3dx10math.h>
#endif

class FCamera
{
public:
	FCamera();
	FCamera(const FCamera& Other);
	~FCamera();

#if DIRECTX
	void SetPosition(float InX, float InY, float InZ);
	void SetPosition(Vector3D InPosition);
	void SetRotation(float InX, float InY, float InZ);
	void SetRotation(Vector3D InRotation);

	void MoveWorldSpace(float InX, float InY, float InZ);
	void MoveWorldSpace(Vector3D InDistance);
	void MoveLocalSpace(float InX, float InY, float InZ);
	void MoveLocalSpace(Vector3D InDistance);
	void Rotate(float InX, float InY, float InZ);
	void Rotate(Vector3D InRotation);

	Vector3D GetPosition() const { return Position; }
	Vector3D GetRotation() const { return Rotation; }
	D3DXVECTOR3 GetD3DPosition();
	D3DXVECTOR3 GetD3DRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX& OutMatrix);

private:
	Vector3D Position;
	Vector3D Rotation;
	D3DXMATRIX ViewMatrix;
#endif

};