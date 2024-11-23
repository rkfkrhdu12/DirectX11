#pragma once


#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// Include //
#include <Windows.h>

class UD3DClass;

class UColorShaderClass;
class UTextureShaderClass;

class ACameraClass;
class AModelClass;

// Global //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.1f;

const int SCREEN_DEFAULTWIDTH = 800;
const int SCREEN_DEFAULTHEIGHT = 600;

class UGraphicsClass
{
public:
	UGraphicsClass() = default;
	UGraphicsClass(const UGraphicsClass&);
	~UGraphicsClass() = default;

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
private:
	bool Render();

private:
	UD3DClass* _d3d = 0;

	// My Class //
	ACameraClass* _camera = 0;
	AModelClass* _model = 0;

	UColorShaderClass* _colorShader = 0;
	UTextureShaderClass* _textureShader = 0;
};

#endif  // ! _GRAPHICSCLASS_H_
