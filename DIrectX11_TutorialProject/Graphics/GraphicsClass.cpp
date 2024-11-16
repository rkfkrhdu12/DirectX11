#include "GraphicsClass.h"

UGraphicsClass::UGraphicsClass()
{
}

UGraphicsClass::UGraphicsClass(const UGraphicsClass&)
{
}

UGraphicsClass::~UGraphicsClass()
{
}

bool UGraphicsClass::Initialize(int, int, HWND)
{
	return true;
}

void UGraphicsClass::Shutdown()
{
}

bool UGraphicsClass::Frame()
{
	return true;
}

bool UGraphicsClass::Render()
{
	return true;
}
