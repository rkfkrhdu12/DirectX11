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

	// 수직동기화 //
	_isVsyncEnabled = vsync;

	// DX 그래픽 인터페이스 팩토리 //
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) return false;

	// 첫번째 그래픽 카드에 대한 어댑터 //
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;

	// 출력에 대한 첫번째 어댑터 세팅 //
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) return false;

	// 해당 포맷과 모니터 디스플레이 출력에 맞는 모드의 개수 갖고오기 //
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result)) return false;

	// 가능한 모든 모니터와 그래픽카드 조합을 저장 //
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	// 디스플레이 모드에 대한 리스트 //
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) return false;

	// 화면에 맞는 디스플레이 모드 찾고 적합한 것의 모니터 새로고침 비율의 분모 분자 값을 저장 (모니터 주사율) //
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

	// 그래픽카드의 값(description)을 저장 //
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	// 현재 그래픽카드 메모리 용량을 메가바이트 단위로 저장 //
	_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 그래픽카드의 이름이 있는지를 통해 에러 체크 //
	error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0) return false;

	// 할당 해제 //
	delete[] displayModeList;
	displayModeList = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;
	// - 할당 해제 //

// 더블버퍼 //
	
	// 스왑 체인 초기화 //
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	
	// 백버퍼 개수 지정 - 1개 //
	swapChainDesc.BufferCount = 1;

	// 버퍼의 사이즈 지정 //
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;

	// 버퍼의 포맷 32비트 포맷
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 수직동기화 설정 //
	if (_isVsyncEnabled)
	{ // 프레임을 모니터 주사율에 고정 //
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{ // 그냥 되는데로 업데이트 //
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// 버퍼의 용도 설정 //
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	
	// 렌더링을 할 윈도우의 핸들 설정 //
	swapChainDesc.OutputWindow = hWnd;

	// 멀티 샘플링 끄기 - 안티엘리어싱 //
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// 창모드 전체화면모드 설정
	swapChainDesc.Windowed = !isFullScreen;

	// 스캔라인과 관련 내용을 지정하지 않음으로 정의 //
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 출력 이후 백버퍼 내용 버리기로 정의 //
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가 옵션 사용하지 않음으로 정의 //
	swapChainDesc.Flags = 0;

	// (단순하게) DX 버전 //
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// 스왑체인, 디바이스, 디바이스컨텍스트 생성 //
	// 디바이스, 디바이스컨텍스트 - 그냥 디바이스 기능을 쪼개 놓은거
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, 
		NULL, &_deviceContext);
	if (FAILED(result)) return false;

	// 백버퍼의 포인터 가져오기 //
	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result)) return false;

	// 렌더타겟 뷰 생성 //
	result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
	if (FAILED(result)) return false;

	// 할당 해제 //
	backBufferPtr->Release();
	backBufferPtr = 0;

	// 깊이 버퍼 초기화 // 
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// 깊이 버퍼의 값을 설정 //
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

	// 깊이버퍼의 텍스처 생성 // 
	result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if (FAILED(result)) return false;

	// 스텐실 상태 초기화 //
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 설정 //
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = depthStencilDesc.StencilWriteMask = 0xFF;

	// 전면에 있을때 작동 방식 정의 //
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 후면에 있을때 작동 방식 정의 //
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 스텐실 상태 디바이스 생성 //
	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if (FAILED(result)) return false;

	// 스텐실 상태 디바이스 설정
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	// 스텐실 뷰 초기화 //
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// 설정 //
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 생성 //
	result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if (FAILED(result)) return false;

	// 렌더타겟뷰와 깊이스텐실버퍼를 렌더파이프라인에 적용 //
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	// 래스터화 - 벡터 그래픽을 래스터 이미지(픽셀, 점, 선)으로 표현(변환) //
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

	// 생성
	result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if (FAILED(result)) return false;

	_deviceContext->RSSetState(_rasterState);

	// 뷰포트 - 화면에 보일 공간 크기 //
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	_deviceContext->RSSetViewports(1, &viewport);

	// 투영 행렬 - 3D를 2D 뷰포트로 표현(변환) //
	fieldOfView = (float)D3DX_PI / 4.f;
	screenAspect = (float)width / (float)height;

	D3DXMatrixPerspectiveFovLH(&_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	// 월드 행렬 - 3D좌표계에서 사용 - 회전|이동|크기를 변환 //
	D3DXMatrixIdentity(&_worldMatrix);

	// 뷰 행렬 - 카메라가 바라보는 방향을 계산 //
	// - 카메라 클래스(CameraClass)로 이동 - //

	// 직교 투영 행렬 - 2D좌표계에서 사용 //
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
