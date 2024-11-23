#pragma once
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

// Include //
#include <D3DX10math.h>

class ACameraClass
{
public:
	ACameraClass() = default;
	~ACameraClass() = default;
	ACameraClass(const ACameraClass&);

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);
private:
	float _positionX = 0, _positionY = 0, _positionZ = 0;
	float _rotationX = 0, _rotationY = 0, _rotationZ = 0;
	D3DXMATRIX _viewMatrix;
};

#endif // !_CAMERACLASS_H_
