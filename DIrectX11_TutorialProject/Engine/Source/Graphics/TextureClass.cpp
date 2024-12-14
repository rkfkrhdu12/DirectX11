#include "TextureClass.h"

UTextureClass::UTextureClass(const UTextureClass&)
{
}

bool UTextureClass::Initialize(ID3D11Device* device, WCHAR* fileName)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &_texture, NULL);
	if (FAILED(result)) return false;

	return true;
}

void UTextureClass::Shutdown()
{
	if (_texture)
	{
		_texture->Release();
		_texture = 0;
	}

}

ID3D11ShaderResourceView* UTextureClass::GetTexture()
{
	return _texture;
}
