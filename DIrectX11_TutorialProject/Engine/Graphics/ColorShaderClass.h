#pragma once

#ifndef _COLORSHADERCLASS_H_

// Include //
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

using namespace std;

class UColorShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	UColorShaderClass() = default;
	UColorShaderClass(const UColorShaderClass&);
	~UColorShaderClass() = default;

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);
	
private:
	ID3D11VertexShader* _vertexShader = 0;
	ID3D11PixelShader* _pixelShader = 0;
	ID3D11InputLayout* _layout = 0;
	ID3D11Buffer* _matrixBuffer = 0;
};

#endif // !_COLORSHADERCLASS_H_
