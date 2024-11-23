#include "GraphicsClass.h"

// MyClass Include
#include "D3D/D3DClass.h"
#include "Shader/ColorShaderClass.h"
#include "Shader/TextureShaderClass.h"

#include "CameraClass.h"
#include "ModelClass.h"


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

	//result = _model->Initialize(_d3d->GetDevice());
	//if (!result)
	//{
	//	MessageBox(hWnd, L"Could not Initialize the model object", L"Error", MB_OK);
	//	return false;
	//}
	//
	//_colorShader = new UColorShaderClass;
	//if (!_colorShader) return false;
	//
	//result = _colorShader->Initialize(_d3d->GetDevice(), hWnd);
	//if (!result)
	//{
	//	MessageBox(hWnd, L"Could not Initialize the ColorShader Object", L"Error", MB_OK);
	//	return false;
	//}

	result = _model->Initialize(_d3d->GetDevice(), L"SampleTexture.dds");
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// ColorShader
	_colorShader = new UColorShaderClass;
	if (!_colorShader) return false;
	
	result = _colorShader->Initialize(_d3d->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not Initialize the ColorShader Object", L"Error", MB_OK);
		return false;
	}

	// TextureShader
	_textureShader = new UTextureShaderClass;
	if (!_textureShader) return false;

	result = _textureShader->Initialize(_d3d->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void UGraphicsClass::Shutdown()
{
	if (_textureShader)
	{
		_textureShader->Shutdown();
		delete _textureShader;
		_textureShader = 0;
	}

	if (_colorShader)
	{
		_colorShader->Shutdown();
		delete _colorShader;
		_colorShader = 0;
	}

	if (_model)
	{
		_model->Shutdown();
		delete _model;
		_model = 0;
	}

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
	return Render();
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

	int i = 2;
	//for (; i <= 2; ++i)
	{
		_model->Render(_d3d->GetDeviceContext(), i);

		result = _colorShader->Render(_d3d->GetDeviceContext(), _model->GetIndexCount(), worldMat, viewMat, projectionMat);
		// result = _textureShader->Render(_d3d->GetDeviceContext(), _model->GetIndexCount(), worldMat, viewMat, projectionMat, _model->GetTexture());
		if (!result) return false;
	}

	_d3d->EndScene();

	return true;
}
