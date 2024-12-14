#include "CameraClass.h"

ACameraClass::ACameraClass(const ACameraClass&)
{
}

void ACameraClass::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}

void ACameraClass::SetRotation(float x, float y, float z)
{
	_rotationX = x;
	_rotationY = y;
	_rotationZ = z;
}

D3DXVECTOR3 ACameraClass::GetPosition()
{
	return D3DXVECTOR3(_positionX, _positionY, _positionZ);
}

D3DXVECTOR3 ACameraClass::GetRotation()
{
	return D3DXVECTOR3(_rotationX, _rotationY, _rotationZ);
}

void ACameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;

	position.x = _positionX;
	position.y = _positionY;
	position.z = _positionZ;

	lookAt.x = 0.f;
	lookAt.y = 0.f;
	lookAt.z = 1.f;

	pitch = _rotationX * 0.0174532925f;
	yaw = _rotationY * 0.0174532925f;
	roll = _rotationZ * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&_viewMatrix, &position, &lookAt, &up);
}

void ACameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = _viewMatrix;
}
