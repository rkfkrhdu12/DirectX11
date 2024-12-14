#include "ModelClass.h"

#include "TextureClass.h"

AModelClass::AModelClass(const AModelClass&)
{
}

bool AModelClass::Initialize(ID3D11Device* device, WCHAR* textureFilename)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result) return false;

	result = LoadTexture(device, textureFilename);
	if (!result) return false;

	return true;
}

bool AModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	result = LoadModel(modelFilename);
	if (!result) return false;

	result = InitializeBuffers(device);
	if (!result) return false;

	result = LoadTexture(device, textureFilename);
	if (!result) return false;

	return true;
}

void AModelClass::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffers();

	ReleaseModel();
}

void AModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int AModelClass::GetIndexCount()
{
	return _indexCount;
}

ID3D11ShaderResourceView* AModelClass::GetTexture()
{
	return _texture->GetTexture();
}

bool AModelClass::InitializeBuffers(ID3D11Device* device)
{
	// VertexType* vertices;
	// unsigned long* indices;
	// D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	// D3D11_SUBRESOURCE_DATA vertexData, indexData;
	// HRESULT result;
	// 
	// _vertexCount = 3;
	// _indexCount = 3;
	// 
	// vertices = new VertexType[_vertexCount];
	// if (!vertices) return false;
	// 
	// indices = new unsigned long[_indexCount];
	// if (!indices) return false;
	// 
	// int arrIndex = 0;
	// vertices[arrIndex].position = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);  // Bottom left.
	// vertices[arrIndex].texture = D3DXVECTOR2(0.0f, 1.0f);
	// vertices[arrIndex++].normal = D3DXVECTOR3(0.0f, 0.0f, -1.f);
	// 
	// vertices[arrIndex].position = D3DXVECTOR3(2.0f, 2.0f, 0.0f);  // Top middle.
	// vertices[arrIndex].texture = D3DXVECTOR2(1.f, 0.0f);
	// vertices[arrIndex++].normal = D3DXVECTOR3(0.0f, 0.0f, -1.f);
	// 
	// vertices[arrIndex].position = D3DXVECTOR3(2.0f, -2.0f, 0.0f);  // Bottom right.
	// vertices[arrIndex].texture = D3DXVECTOR2(1.0f, 1.0f);
	// vertices[arrIndex++].normal = D3DXVECTOR3(0.0f, 0.0f, -1.f);
	// 
	// 
	// //vertices[arrIndex].position = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);  // Bottom left.
	// //vertices[arrIndex].texture = D3DXVECTOR2(0.0f, 1.0f);
	// //vertices[arrIndex++].normal = D3DXVECTOR3(0.0f, 0.0f, -1.f);
	// 
	// //vertices[arrIndex].position = D3DXVECTOR3(-2.0f, 2.0f, 0.0f);  // Top Left
	// //vertices[arrIndex].texture = D3DXVECTOR2(0.f, 0.0f);
	// //vertices[arrIndex++].normal = D3DXVECTOR3(0.0f, 0.0f, -1.f);
	// 
	// //vertices[arrIndex].position = D3DXVECTOR3(2.0f, 2.0f, 0.0f);  // Top right.
	// //vertices[arrIndex].texture = D3DXVECTOR2(1.0f, 0.0f);
	// //vertices[arrIndex++].normal = D3DXVECTOR3(0.0f, 0.0f, -1.f);
	// 
	// 
	// indices[0] = 0;  // Bottom left.
	// indices[1] = 1;  // Top middle.
	// indices[2] = 2;  // Bottom right.
	// /*indices[3] = 3;
	// indices[4] = 4;
	// indices[5] = 5;*/
	// 
	// vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	// vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// vertexBufferDesc.CPUAccessFlags = 0;
	// vertexBufferDesc.MiscFlags = 0;
	// vertexBufferDesc.StructureByteStride = 0;
	// 
	// vertexData.pSysMem = vertices;
	// vertexData.SysMemPitch = 0;
	// vertexData.SysMemSlicePitch = 0;
	// 
	// result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	// if (FAILED(result)) return false;
	// 
	// indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	// indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// indexBufferDesc.CPUAccessFlags = 0;
	// indexBufferDesc.MiscFlags = 0;
	// indexBufferDesc.StructureByteStride = 0;
	// 
	// // Give the subresource structure a pointer to the index data.
	// indexData.pSysMem = indices;
	// 
	// // Create the index buffer.
	// result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	// if (FAILED(result)) return false;
	// 
	// // Release the arrays now that the vertex and index buffers have been created and loaded.
	// delete[] vertices;
	// vertices = 0;
	// 
	// delete[] indices;
	// indices = 0;
	// 
	// return true;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new VertexType[_vertexCount];
	if (!vertices) return false;

	indices = new unsigned long[_indexCount];
	if (!indices) return false;
	
	for (int i = 0; i < _vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(_model[i].x, _model[i].y, _model[i].z);
		vertices[i].texture = D3DXVECTOR2(_model[i].tu, _model[i].tv);
		vertices[i].normal = D3DXVECTOR3(_model[i].nx, _model[i].ny, _model[i].nz);

		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void AModelClass::ShutdownBuffers()
{
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = 0;
	}

	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = 0;
	}
}

void AModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool AModelClass::LoadTexture(ID3D11Device* device, WCHAR* fileName)
{
	bool result;

	_texture = new UTextureClass;
	if (!_texture) return false;

	result = _texture->Initialize(device, fileName);
	if (!result) return false;

	return true;
}

void AModelClass::ReleaseTexture()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = 0;
	}
}

bool AModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	fin.open(filename);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> _vertexCount;

	_indexCount = _vertexCount;

	_model = new ModelType[_vertexCount];
	if (!_model)
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (i = 0; i < _vertexCount; i++)
	{
		fin >> _model[i].x >> _model[i].y >> _model[i].z;
		fin >> _model[i].tu >> _model[i].tv;
		fin >> _model[i].nx >> _model[i].ny >> _model[i].nz;
	}

	fin.close();

	return true;
}

void AModelClass::ReleaseModel()
{
	if (_model)
	{
		delete[] _model;
		_model = 0;
	}
}

