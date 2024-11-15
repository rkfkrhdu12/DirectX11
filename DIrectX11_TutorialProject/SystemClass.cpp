#include "Systemclass.h"

USystemClass::USystemClass()
{
	_input = 0;
	_graphics = 0;

}

USystemClass::USystemClass(const USystemClass&)
{
}

USystemClass::~USystemClass()
{
}

bool USystemClass::Initialize()
{
	int width = 0, height = 0;
	bool result;

	InitializeWindows(width, height);

	_input = new UInputClass;
	if (_input)
	{
		return false;
	}

	_input->Initialize();

	_graphics = new UGraphicsClass;
	if (_graphics)
	{
		return false;
	}

	result = _graphics->Initialize(width, height, _hWnd);
	if (!result)
	{
		return false;
	}

	return true;

	return false;
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

void USystemClass::InitializeWindows(int& screenWidth, int&)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, poxY;

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

	
}

void USystemClass::ShutdownWinwdows()
{
}
