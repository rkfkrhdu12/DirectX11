#include "D3DClass.h"

UD3DClass::UD3DClass(const UD3DClass&)
{
}

bool UD3DClass::Initialize(int width, int height, bool vsync, HWND hWnd,
	bool isFullScreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes = 0, numerator = 0, denominator = 0;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;

	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// ��������ȭ //
	_isVsyncEnabled = vsync;

	// DX �׷��� �������̽� ���丮 //
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) return false;

	// ù��° �׷��� ī�忡 ���� ����� //
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;

	// ��¿� ���� ù��° ����� ���� //
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) return false;

	// �ش� ���˰� ����� ���÷��� ��¿� �´� ����� ���� ������� //
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result)) return false;

	// ������ ��� ����Ϳ� �׷���ī�� ������ ���� //
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	// ���÷��� ��忡 ���� ����Ʈ //
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) return false;

	// ȭ�鿡 �´� ���÷��� ��� ã�� ������ ���� ����� ���ΰ�ħ ������ �и� ���� ���� ���� (����� �ֻ���) //
	for (int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)width)
		{
			if (displayModeList[i].Height == (unsigned int)height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// �׷���ī���� ��(description)�� ���� //
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	// ���� �׷���ī�� �޸� �뷮�� �ް�����Ʈ ������ ���� //
	_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �׷���ī���� �̸��� �ִ����� ���� ���� üũ //
	error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0) return false;

	// �Ҵ� ���� //
	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;
	// - �Ҵ� ���� //

// ������� //
	
	// ���� ü�� �ʱ�ȭ //
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	
	// ����� ���� ���� - 1�� //
	swapChainDesc.BufferCount = 1;

	// ������ ������ ���� //
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;

	// ������ ���� 32��Ʈ ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ��������ȭ ���� //
	if (_isVsyncEnabled)
	{ // �������� ����� �ֻ����� ���� //
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{ // �׳� �Ǵµ��� ������Ʈ //
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// ������ �뵵 ���� //
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	
	// �������� �� �������� �ڵ� ���� //
	swapChainDesc.OutputWindow = hWnd;

	// ��Ƽ ���ø� ���� - ��Ƽ������� //
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// â��� ��üȭ���� ����
	swapChainDesc.Windowed = !isFullScreen;

	// ��ĵ���ΰ� ���� ������ �������� �������� ���� //
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ��� ���� ����� ���� ������� ���� //
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰� �ɼ� ������� �������� ���� //
	swapChainDesc.Flags = 0;

	// (�ܼ��ϰ�) DX ���� //
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// ����ü��, ����̽�, ����̽����ؽ�Ʈ ���� //
	// ����̽�, ����̽����ؽ�Ʈ - �׳� ����̽� ����� �ɰ� ������
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, 
		NULL, &_deviceContext);
	if (FAILED(result)) return false;

	// ������� ������ �������� //
	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result)) return false;

	// ����Ÿ�� �� ���� //
	result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
	if (FAILED(result)) return false;

	// �Ҵ� ���� //
	backBufferPtr->Release();
	backBufferPtr = 0;

	// ���� ���� �ʱ�ȭ // 
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// ���� ������ ���� ���� //
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// ���̹����� �ؽ�ó ���� // 
	result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if (FAILED(result)) return false;

	// ���ٽ� ���� �ʱ�ȭ //
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// ���� //
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = depthStencilDesc.StencilWriteMask = 0xFF;

	// ���鿡 ������ �۵� ��� ���� //
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �ĸ鿡 ������ �۵� ��� ���� //
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ���ٽ� ���� ����̽� ���� //
	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if (FAILED(result)) return false;

	// ���ٽ� ���� ����̽� ����
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	// ���ٽ� �� �ʱ�ȭ //
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// ���� //
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ���� //
	result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if (FAILED(result)) return false;

	// ����Ÿ�ٺ�� ���̽��ٽǹ��۸� �������������ο� ���� //
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	// ������ȭ - ���� �׷����� ������ �̹���(�ȼ�, ��, ��)���� ǥ��(��ȯ) //
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.f;

	// ����
	result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if (FAILED(result)) return false;

	_deviceContext->RSSetState(_rasterState);

	// ����Ʈ - ȭ�鿡 ���� ���� ũ�� //
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	_deviceContext->RSSetViewports(1, &viewport);

	// ���� ��� - 3D�� 2D ����Ʈ�� ǥ��(��ȯ) //
	fieldOfView = (float)D3DX_PI / 4.f;
	screenAspect = (float)width / (float)height;

	D3DXMatrixPerspectiveFovLH(&_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// ���� ��� - 3D��ǥ�迡�� ��� - ȸ��|�̵�|ũ�⸦ ��ȯ //
	D3DXMatrixIdentity(&_worldMatrix);

	// �� ��� - ī�޶� �ٶ󺸴� ������ ��� //
	// - ī�޶� Ŭ����(CameraClass)�� �̵� - //

	// ���� ���� ��� - 2D��ǥ�迡�� ��� //
	D3DXMatrixOrthoLH(&_orthoMatrix, (float)width, (float)height, screenNear, screenDepth);

	return true;
}

void UD3DClass::Shutdown()
{
	if (_swapChain)
	{
		_swapChain->SetFullscreenState(false, NULL);
	}

	if (_rasterState)
	{
		_rasterState->Release();
		_rasterState = 0;
	}

	if (_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if (_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = 0;
	}

	if (_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}

	if (_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = 0;
	}

	if (_deviceContext)
	{
		_deviceContext->Release();
		_deviceContext = 0;
	}

	if (_device)
	{
		_device->Release();
		_device = 0;
	}

	if (_swapChain)
	{
		_swapChain->Release();
		_swapChain = 0;
	}


}

void UD3DClass::BeginScene(float R, float G, float B, float A)
{
	float color[4];
	color[0] = R;
	color[1] = G;
	color[2] = B;
	color[3] = A;

	if (!_deviceContext) return;

	_deviceContext->ClearRenderTargetView(_renderTargetView, color);

	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);


}

void UD3DClass::EndScene()
{
	if (_isVsyncEnabled)
	{
		_swapChain->Present(1, 0);
	}
	else
	{
		_swapChain->Present(0, 0);
	}

}

ID3D11Device* UD3DClass::GetDevice()
{
	return _device;
}

ID3D11DeviceContext* UD3DClass::GetDeviceContext()
{
	return _deviceContext;
}

void UD3DClass::GetProjectionMatrix(D3DXMATRIX& p)
{
	p = _projectionMatrix;
}

void UD3DClass::GetWorldMatrix(D3DXMATRIX& w)
{
	w = _worldMatrix;
}

void UD3DClass::GetOrthoMatrix(D3DXMATRIX& o)
{
	o = _orthoMatrix;
}

void UD3DClass::GetVideoCardInfo(char* name, int& mem)
{
	strcpy_s(name, 128, _videoCardDescription);
	mem = _videoCardMemory;
}
