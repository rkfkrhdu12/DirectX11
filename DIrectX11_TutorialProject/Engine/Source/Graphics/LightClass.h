#pragma once

#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <D3DX10math.h>

class ULightClass
{
public:
	ULightClass() = default;
	~ULightClass() = default;
	ULightClass(const ULightClass&);

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 _ambientColor;
	D3DXVECTOR4 _diffuseColor;
	D3DXVECTOR3 _direction;
};

#endif