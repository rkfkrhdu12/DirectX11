#pragma once

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


// Win32 헤더 제외
#define WIN32_LEAN_AND_MEAN

// Include //
#include <Windows.h>


// MyClass Include //
class UInputClass;
class UGraphicsClass;

class USystemClass
{
public:
	USystemClass() = default;
	USystemClass(const USystemClass&);
	~USystemClass() = default;

	bool Initialize();
	void Shutdown();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWinwdows();

private:
	LPCWSTR _applicationName;
	HINSTANCE _hInstance;
	HWND _hWnd;

	UInputClass* _input = NULL;
	UGraphicsClass* _graphics = NULL;

};


// Function //
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GlobalHandle //
static USystemClass* hApplication = nullptr;


#endif


