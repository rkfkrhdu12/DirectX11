#pragma once

#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

// Include //
#include <d3d11.h>
#include <d3dx11tex.h>

class UTextureClass
{

public:
	UTextureClass() = default;
	~UTextureClass() = default;
	UTextureClass(const UTextureClass&);

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* _texture = 0;
};

#endif