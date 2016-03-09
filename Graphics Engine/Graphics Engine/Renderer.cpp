#include "Renderer.h"

#include "Color.h"

// Instantiate static instance
Renderer* Renderer::s_Instance = nullptr;

// ===== Static Interface ===== //
Renderer* Renderer::GetInstance()
{
	if (s_Instance == nullptr)
	{
		s_Instance = new Renderer;
	}
	return s_Instance;
}
// ============================ //

// ===== Interface ===== //
void Renderer::Initialize(HWND _window, const int _samplerCount, const float _screenHeight, const float _screenWidth)
{
	CreateDeviceAndSwapChain(_window);
	CreateRTV();
	CreateDSV((int)_screenWidth, (int)_screenHeight);
	SetupViewports(_screenWidth, _screenHeight);

	// === Float4x4 Initialization (Probably will be moved later)
	CreateViewMatrix();
	m_ProjectionMatrix = CreateProjectionMatrix(65.0f, _screenWidth, _screenHeight);
}

void Renderer::Terminate()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(RTV);
	SAFE_RELEASE(DSV);
	SAFE_RELEASE(depthStencil);
	SAFE_RELEASE(deviceContext);

	delete s_Instance;
	s_Instance = nullptr;
}

void Renderer::Render()
{
	// === Clear the Screen
	deviceContext->RSSetViewports(1, &viewport);
	deviceContext->OMSetRenderTargets(1, &RTV, DSV);
	deviceContext->ClearRenderTargetView(RTV, Colors::BLUE.RGBA);
	deviceContext->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH, 1, NULL);

	// === Render the Stuff in the Scene
	RenderNode* currNode = m_RenderSet.GetFront();
	while (currNode != nullptr) {
		currNode->RenderProcess();
		currNode = currNode->m_Next;
	}

	swapChain->Present(0, 0);
}

void Renderer::AddForRendering(RenderContext* _rContext, RenderMaterial* _rMaterial, RenderShape* _rShape)
{
	// === Does it Already Contain the Context?
	RenderNode* contextNode = m_RenderSet.Find(_rContext);
	if (contextNode != nullptr) {
		contextNode->Add(_rMaterial, _rShape);
	}
	else {
		m_RenderSet.Add(_rContext);
		_rContext->Add(_rMaterial, _rShape);
	}
}
// ===================== //

// === Private Interface === //
XMFLOAT4X4 Renderer::CreateProjectionMatrix(float _fov, float _width, float _height) 
{
	float nearPlane = 0.1f, farPlane = 100;
	float verticalFOV = XMConvertToRadians(_fov), aspectRatio = _width / _height;
	float yScale = (1 / tan(verticalFOV * 0.5f)), xScale = yScale / aspectRatio;

	return XMFLOAT4X4
		(xScale, 0,      0,                                                  0,
		 0,      yScale, 0,                                                  0,
		 0,      0,      farPlane / (farPlane - nearPlane),                  1,
		 0,      0,      (-(farPlane * nearPlane)) / (farPlane - nearPlane), 0);
}

void Renderer::CreateViewMatrix() 
{
	m_ViewMatrix = XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, -2, 1);
	XMMATRIX matrix = XMLoadFloat4x4(&m_ViewMatrix);
	XMVECTOR determinant = XMMatrixDeterminant(matrix);
	matrix = XMMatrixInverse(&determinant, matrix);
	XMStoreFloat4x4(&m_ViewMatrix, matrix);
}
// ========================= //

// ===== DirectX Initialization ===== //
void Renderer::CreateDeviceAndSwapChain(HWND _window)
{
	const int WIDTH = 1024;
	const int HEIGHT = 780;
	const int SAMPLE_COUNT = 1;

	UINT flag = NULL;
#if _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC description;
	ZeroMemory(&description, sizeof(DXGI_SWAP_CHAIN_DESC));
	description.BufferCount = 1;
	description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	description.BufferDesc.Height = HEIGHT;
	description.BufferDesc.Width = WIDTH;
	description.BufferDesc.RefreshRate.Denominator = 1;
	description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.OutputWindow = _window;
	description.Windowed = true;
	description.SampleDesc.Count = SAMPLE_COUNT;
	description.Flags = NULL;
//	description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, flag,
		0, 0, D3D11_SDK_VERSION, &description, &swapChain, &device, 0, &deviceContext);
	assert(hr == S_OK);
}

void Renderer::CreateRTV()
{
	HRESULT hr;

	ID3D11Resource* pBackBuffer = nullptr;
	swapChain->GetBuffer(0, __uuidof(pBackBuffer), reinterpret_cast<void**>(&pBackBuffer));

	hr = device->CreateRenderTargetView(pBackBuffer, nullptr, &RTV);
	SAFE_RELEASE(pBackBuffer);

	assert(hr == S_OK);
}

void Renderer::CreateDSV(int _width, int _height)
{
	// === Create the Depth-Stencil
	D3D11_TEXTURE2D_DESC depthDescription;
	depthDescription.Width = _width;
	depthDescription.Height = _height;
	depthDescription.MipLevels = 1;
	depthDescription.ArraySize = 1;
	depthDescription.Format = DXGI_FORMAT_D32_FLOAT;
	depthDescription.SampleDesc.Count = 1;
	depthDescription.SampleDesc.Quality = 0;
	depthDescription.Usage = D3D11_USAGE_DEFAULT;
	depthDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDescription.CPUAccessFlags = 0;
	depthDescription.MiscFlags = 0;

	device->CreateTexture2D(&depthDescription, NULL, &depthStencil);

	// === Create the Depth-Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC viewDescription;
	ZeroMemory(&viewDescription, sizeof(viewDescription));
	viewDescription.Format = DXGI_FORMAT_D32_FLOAT;
	viewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	viewDescription.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(depthStencil, &viewDescription, &DSV);
}

void Renderer::SetupViewports(float _width, float _height)
{
	viewport.Height   = _height;
	viewport.Width    = _width;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
}
// ================================== //
