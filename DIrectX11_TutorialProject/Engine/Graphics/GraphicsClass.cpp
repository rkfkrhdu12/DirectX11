#include "GraphicsClass.h"

// MyClass Include
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"

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

	_camera = new ACameraClass();
	if (!_camera) return false;

	_camera->SetPosition(0.f, 0.f, -10.f);

	_model = new AModelClass();
	if (!_model) return false;

	result = _model->Initialize(_d3d->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, L"Could not Initialize the model object", L"Error", MB_OK);
		return false;
	}

	_colorShader = new UColorShaderClass;
	if (!_colorShader) return false;

	result = _colorShader->Initialize(_d3d->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not Initialize the ColorShader Object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void UGraphicsClass::Shutdown()
{
	if (_colorShader)
	{
		_colorShader->Shutdown();
		delete _colorShader;
		_colorShader = 0;
	}

	// Release the model object.
	if (_model)
	{
		_model->Shutdown();
		delete _model;
		_model = 0;
	}

	// Release the camera object.
	if (_camera)
	{
		delete _camera;
		_camera = 0;
	}

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
	D3DXMATRIX worldMat, viewMat, projectionMat;
	bool result;

	_d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	_camera->Render();

	_camera->GetViewMatrix(viewMat);
	_d3d->GetWorldMatrix(worldMat);
	_d3d->GetProjectionMatrix(projectionMat);

	_model->Render(_d3d->GetDeviceContext());

	result = _colorShader->Render(_d3d->GetDeviceContext(), _model->GetIndexCount(), worldMat, viewMat, projectionMat);
	if (!result) return false;

	_d3d->EndScene();

	return true;
}
