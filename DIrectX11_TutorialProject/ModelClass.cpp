#include "ModelClass.h"

AModelClass::AModelClass(const AModelClass& other)
{
}

bool AModelClass::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result) return false;

	return true;
}

void AModelClass::Shutdown()
{
	ShutdownBuffers();
}

void AModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);


}

int AModelClass::GetIndexCount()
{
	return _indexCount;
}

bool AModelClass::InitializeBuffers(ID3D11Device*)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	_vertexCount = 3;
	_indexCount = 3;

	vertices = new VertexType[_vertexCount];
	if (!vertices) return false;

	// Save https://ppparkje.tistory.com/13

	return true;
}

void AModelClass::ShutdownBuffers()
{
}

void AModelClass::RenderBuffers(ID3D11DeviceContext*)
{
}
