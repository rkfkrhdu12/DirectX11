#pragma once

#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_

// Include //
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

using namespace std;

class UTextureShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	UTextureShaderClass() = default;
	~UTextureShaderClass() = default;
	UTextureShaderClass(const UTextureShaderClass&);

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* _vertexShader = 0;
	ID3D11PixelShader* _pixelShader = 0;
	ID3D11InputLayout* _layout = 0;
	ID3D11Buffer* _matrixBuffer = 0;

	ID3D11SamplerState* _sampleState = 0;
};

#endif