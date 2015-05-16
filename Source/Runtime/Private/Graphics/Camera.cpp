/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/Camera.h"

FCamera::FCamera()
{
#if DIRECTX
	PosX = 0.0f;
	PosY = 0.0f;
	PosZ = 0.0f;

	RotX = 0.0f;
	RotY = 0.0f;
	RotZ = 0.0f;
#endif 
}

FCamera::FCamera(const FCamera& Other)
{

}

FCamera::~FCamera()
{

}

#if DIRECTX

void FCamera::SetPosition(float InX, float InY, float InZ)
{
	PosX = InX;
	PosY = InY;
	PosZ = InZ;
}

void FCamera::SetRotation(float InX, float InY, float InZ)
{
	RotX = InX;
	RotY = InY;
	RotZ = InZ;
}

D3DXVECTOR3 FCamera::GetPosition()
{
	return D3DXVECTOR3(PosX, PosY, PosZ);
}

D3DXVECTOR3 FCamera::GetRotation()
{
	return D3DXVECTOR3(RotX, RotY, RotZ);
}

void FCamera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	// Set the up vector
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Set the position of the camera
	position = GetPosition();

	// Setup where the camera is looking
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the rotation in radians
	pitch = RotX * RotToRad;
	yaw = RotY * RotToRad;
	roll = RotZ * RotToRad;

	// Create the rotation matrix
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt += position;

	// Create the view matrix from the three updated vectors
	D3DXMatrixLookAtLH(&ViewMatrix, &position, &lookAt, &up);
}

void FCamera::GetViewMatrix(D3DXMATRIX& OutMatrix)
{
	OutMatrix = ViewMatrix;
}

#endif