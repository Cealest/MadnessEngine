/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Graphics/Camera.h"

FCamera::FCamera()
{
	Position = 0.0f;
	Rotation = 0.0f;
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
	Position.X = InX;
	Position.Y = InY;
	Position.Z = InZ;
}

void FCamera::SetPosition(Vector3D InPosition)
{
	Position = InPosition;
}

void FCamera::SetRotation(float InX, float InY, float InZ)
{
	Rotation.X = InX;
	Rotation.Y = InY;
	Rotation.Z = InZ;
}

void FCamera::SetRotation(Vector3D InRotation)
{
	Rotation = InRotation;
}

void FCamera::MoveWorldSpace(float InX, float InY, float InZ)
{
	Position.X += InX;
	Position.Y += InY;
	Position.Z += InZ;
}

void FCamera::MoveWorldSpace(Vector3D InDistance)
{
	Position += InDistance;
}

void FCamera::MoveLocalSpace(float InX, float InY, float InZ)
{
	if (InX == 0 && InY == 0 && InZ == 0)
		return;

	Matrix4x4 origin = Matrix4x4::Identity();
	origin.Translate(Position);
	origin.Rotate(Rotation);

	Position += origin * Vector3D(InX, InY, InZ);
}

void FCamera::MoveLocalSpace(Vector3D InDistance)
{
	if (InDistance.X == 0 && InDistance.Y == 0 && InDistance.Z == 0)
		return;

	Matrix4x4 origin = Matrix4x4::Identity();
	origin.Translate(Position);
	origin.Rotate(Rotation);

	Position += origin * InDistance;
}

void FCamera::Rotate(float InX, float InY, float InZ)
{
	Rotation.X += InX;
	Rotation.Y += InY;
	Rotation.Z += InZ;
}

void FCamera::Rotate(Vector3D InRotation)
{
	Rotation += InRotation;
}

D3DXVECTOR3 FCamera::GetD3DPosition()
{
	return D3DXVECTOR3(Position.X, Position.Y, Position.Z);
}

D3DXVECTOR3 FCamera::GetD3DRotation()
{
	return D3DXVECTOR3(Rotation.X, Rotation.Y, Rotation.Z);
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
	position = GetD3DPosition();

	// Setup where the camera is looking
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the rotation in radians
	pitch = Rotation.X * RotToRad;
	yaw = Rotation.Y * RotToRad;
	roll = Rotation.Z * RotToRad;

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