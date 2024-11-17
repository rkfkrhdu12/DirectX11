#pragma once

#ifndef _D3DClass_H_

// Link //
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

// Include //
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>
// #include "Windows.h"

class UD3DClass
{
public:
	UD3DClass() = default;
	UD3DClass(const UD3DClass&);
	~UD3DClass() = default;

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

private:
	bool _isVsyncEnabled = false;
	bool _videoCardMemory = false;
	char _videoCardDescription[128] = {0};
	IDXGISwapChain* _swapChain = 0;
	ID3D11Device* _device = 0;
	ID3D11DeviceContext* _deviceContext = 0;
	ID3D11RenderTargetView* _renderTargetView = 0;
	ID3D11Texture2D* _depthStencilBuffer = 0;
	ID3D11DepthStencilState* _depthStencilState = 0;
	ID3D11DepthStencilView* _depthStencilView = 0;
	ID3D11RasterizerState* _rasterState = 0;
	D3DXMATRIX _projectionMatrix;
	D3DXMATRIX _worldMatrix;
	D3DXMATRIX _orthoMatrix;
};


#endif // !_D3DClass_H_
