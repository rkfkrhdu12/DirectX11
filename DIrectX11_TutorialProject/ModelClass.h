#pragma once

#ifndef _MODELCLASS_H_


// Inlcude //
#include <d3d11.h>
#include <D3DX10math.h>

class AModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

public:
	AModelClass() = default;
	~AModelClass() = default;
	AModelClass(const AModelClass&);

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* _vertexBuffer = 0, *_indexBuffer = 0;
	int _vertexCount = 0, _indexCount = 0;
};

#endif // !_MODELCLASS_H_