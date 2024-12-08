#include "LightClass.h"

ULightClass::ULightClass(const ULightClass&)
{
}

void ULightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	_diffuseColor = D3DXVECTOR4(r, g, b, a);
}

void ULightClass::SetDirection(float x, float y, float z)
{
	_direction = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR4 ULightClass::GetDiffuseColor()
{
	return _diffuseColor;
}

D3DXVECTOR3 ULightClass::GetDirection()
{
	return _direction;
}
