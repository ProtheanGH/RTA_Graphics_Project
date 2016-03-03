#include "Renderer.h"

#define SAFE_RELEASE(pointer) {if(pointer){pointer->Release(); pointer = nullptr;}}

// Instantiate static instance
Renderer* Renderer::m_instance = nullptr;


/*static*/ Renderer* Renderer::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new Renderer;
	return m_instance;
}

/*static*/ void Renderer::DeleteInstance()
{
	delete m_instance;
	m_instance = nullptr;
}

void Renderer::CreateDeviceAndSwapChain(HWND _window)
{
	const int WIDTH = 1024;
	const int HEIGHT = 780;
	const int SAMPLE_COUNT = 2;
	DXGI_SWAP_CHAIN_DESC description;
	ZeroMemory(&description, sizeof(DXGI_SWAP_CHAIN_DESC));
	description.BufferCount       = 1;
	description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	description.BufferDesc.Height = HEIGHT;
	description.BufferDesc.Width  = WIDTH;
	description.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.OutputWindow      = _window;
	description.SampleDesc.Count  = SAMPLE_COUNT;
	description.SwapEffect        = DXGI_SWAP_EFFECT_DISCARD;
	description.Windowed          = true;


	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
										0, 0, D3D11_SDK_VERSION, &description, &swapChain, &device, 0, &deviceContext);
	assert(hr == S_OK);
}

void Renderer::CreateRTV()
{
	HRESULT hr;

	ID3D11Resource* local = nullptr;
	swapChain->GetBuffer(0, __uuidof(local), reinterpret_cast<void**>(&local));

	hr = device->CreateRenderTargetView(local, nullptr, &RTV);
	assert(hr == S_OK);
}

void Renderer::CreateDSV(ID3D11Texture2D* _depthStencil)
{
	HRESULT hr;
	hr = device->CreateDepthStencilView(_depthStencil, nullptr, &DSV);
	assert(hr == S_OK);
}

void Renderer::CreateInputElementDescription()
{
	//	inputElementDesc[] =
	//	{
	//		{ "NAME", 0, FORMAT, 0, ELEMENT, DATA, 0 }
	//	};


	//	inputElementDesc[] =
	//	{
	//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//		{ "UV",       0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//	};
}

void Renderer::CreateSamplerState(const D3D11_SAMPLER_DESC &_description)
{
	HRESULT hr;
	hr = device->CreateSamplerState(&_description, &samplerState);

	assert(hr == S_OK);
}

void Renderer::Initialize(HWND _window, const int _samplerCount, const int _screenHeight, const int _screenWidth)
{
	HRESULT hr;

	CreateDeviceAndSwapChain(_window);
	CreateRTV();

	// === Texture === //
	ID3D11Texture2D* depthStencil = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width              = _screenWidth;
	textureDesc.Height             = _screenHeight;
	textureDesc.MipLevels          = 1;
	textureDesc.ArraySize          = 1;
	textureDesc.Format             = DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count   = _samplerCount;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage              = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags     = 0;
	textureDesc.MiscFlags          = 0;
	// ===

	hr = device->CreateTexture2D(&textureDesc, nullptr, &depthStencil);
	assert(hr == S_OK);
	hr = device->CreateDepthStencilView(depthStencil, nullptr, &DSV);
	assert(hr == S_OK);

	// === Sampler Description === //
	D3D11_SAMPLER_DESC description;
	ZeroMemory(&description, sizeof(D3D11_SAMPLER_DESC));
	description.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
	description.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
	description.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
	description.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	description.MinLOD         = -FLT_MAX;
	description.MaxLOD         = FLT_MAX;
	description.MipLODBias     = 0.0f;
	description.MaxAnisotropy  = 1;
	description.ComparisonFunc = D3D11_COMPARISON_NEVER;
	description.BorderColor[0] = 1.0f;
	description.BorderColor[1] = 1.0f;
	description.BorderColor[2] = 1.0f;
	description.BorderColor[3] = 1.0f;
	// ===


	CreateDSV(depthStencil);
	CreateInputElementDescription();
	CreateSamplerState(description);
}

void Renderer::Terminate()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(RTV);
	SAFE_RELEASE(DSV);
	SAFE_RELEASE(deviceContext);
	// SAFE_RELEASE(SRV); NOT BEING USED
	SAFE_RELEASE(samplerState);
}

void Renderer::Render() const
{

}

