#include "Systemclass.h"

// My Class Include //
#include "../Input/InputClass.h"
#include "../Graphics/GraphicsClass.h"


USystemClass::USystemClass(const USystemClass&)
{
}

bool USystemClass::Initialize()
{
	int width = 0, height = 0;
	bool result;

	InitializeWindows(width, height);

	_input = new UInputClass;
	if (!_input) return false;

	_input->Initialize();

	_graphics = new UGraphicsClass;
	if (!_graphics) return false;

	result = _graphics->Initialize(width, height, _hWnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void USystemClass::Shutdown()
{
	if (_graphics)
	{
		_graphics->Shutdown();
		delete _graphics;
		_graphics = 0;
	}

	if (_input)
	{
		_input->Shutdown();
		delete _input;
		_input = 0;
	}

	ShutdownWinwdows();
}

void USystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(msg));

	done = false;
	while (!done)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

LRESULT USystemClass::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		_input->KeyDown((unsigned int)wParam);
		return 0;
	}
	case WM_KEYUP:
	{
		_input->KeyUp((unsigned int)wParam);
		return 0;
	}

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

bool USystemClass::Frame()
{
	bool result;

	if (_input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = _graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

void USystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	hApplication = this;

	_hInstance = GetModuleHandle(NULL);

	_applicationName = L"Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	_hWnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, _hInstance, NULL);

	ShowWindow(_hWnd, SW_SHOW);
	SetForegroundWindow(_hWnd);
	SetFocus(_hWnd);

	ShowCursor(false);
}

void USystemClass::ShutdownWinwdows()
{
	ShowCursor(true);

	if (FULL_SCREEN) ChangeDisplaySettings(NULL, 0);

	DestroyWindow(_hWnd);
	_hWnd = NULL;

	UnregisterClass(_applicationName, _hInstance);
	_hInstance = NULL;

	hApplication = NULL;
}

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return hApplication->MessageHandler(hWnd, msg, wParam, lParam);
	}
	}
}
