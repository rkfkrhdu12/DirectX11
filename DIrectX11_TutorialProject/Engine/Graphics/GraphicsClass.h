#pragma once


#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// Include //
#include <Windows.h>

class UD3DClass;

// Global //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.1f;

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
};

#endif  // ! _GRAPHICSCLASS_H_
