#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

// Include //
#include <d3d11.h>
#include <D3DX10math.h>

class UTextureClass;

class AModelClass
{
private:
	struct VertexType_COLORSHADER
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};
	
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	AModelClass() = default;
	~AModelClass() = default;
	AModelClass(const AModelClass&);

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	void RenderBuffers(ID3D11DeviceContext*, unsigned int);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
private:
	ID3D11Buffer* _vertexBuffer = 0, * _indexBuffer = 0;
	int _vertexCount = 0, _indexCount = 0;

	UTextureClass* _texture = 0;
};

#endif // !_MODELCLASS_H_
