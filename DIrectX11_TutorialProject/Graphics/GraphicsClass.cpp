#include "GraphicsClass.h"

#include "D3DClass.h"

UGraphicsClass::UGraphicsClass(const UGraphicsClass&)
{
}

bool UGraphicsClass::Initialize(int width, int height, HWND hWnd)
{
	bool result;

	_d3d = new UD3DClass;
	if (!_d3d) return false;

	result = _d3d->Initialize(width, height, VSYNC_ENABLED, hWnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, L"Not Initialize D3D", L"Err", MB_OK);
		return false;
	}

	return true;
}

void UGraphicsClass::Shutdown()
{
	if (_d3d)
	{
		_d3d->Shutdown();
		delete _d3d;
		_d3d = NULL;
	}
}

bool UGraphicsClass::Frame()
{
	bool result;

	result = Render();

	return result;
}

bool UGraphicsClass::Render()
{
	_d3d->BeginScene(.5f, .5f, .5f, 1.0f);

	_d3d->EndScene();

	return true;
}
