#pragma once
//////////////////////////////////////////////////////////////////////////////////
// Created By:		Doug Berg
// Last Updated:	3.2.2016
//////////////////////////////////////////////////////////////////////////////////


#include <DirectXMath.h>
using namespace DirectX;


#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")


class Renderer
{
private:
	// === Members
	ID3D11Device             *device;
	IDXGISwapChain           *swapChain;
	ID3D11RenderTargetView   *RTV;
	ID3D11DepthStencilView   *DSV;
	ID3D11DeviceContext      *deviceContext;
	ID3D11ShaderResourceView *SRV;
	ID3D11SamplerState       *samplerState;
	D3D11_VIEWPORT            viewport;
	D3D11_INPUT_ELEMENT_DESC  inputElementDesc[];
	

	// === DirectX Initialization === //
	void CreateDeviceAndSwapChain ( HWND _window );
	void CreateRTV ( void );
	void CreateDSV ( ID3D11Texture2D* _depthStencil );
	void CreateInputElementDescription ( void );
	void CreateSamplerState ( const D3D11_SAMPLER_DESC &_description );
	// ============================== //

public:
	static Renderer* m_instance;

	// === Constructor / Destructor === //
	Renderer  ( void ) = default;
	~Renderer ( void ) = default;
	Renderer(const Renderer& _copy)                = delete;
	Renderer& operator = (const Renderer& _assign) = delete;
	
	// ================================ //

	// === SINGLETON CLASS === //
	static Renderer* GetInstance ( void );
	static void DeleteInstance   ( void );
	void Initialize( HWND _window, const int _samplerCount, const int _screenHeight, const int _screenWidth );
	void Terminate();
	// ======================= //


	// === Interface === //
	void Render() const;
	// ================= //

	// === Accessors === //
	inline ID3D11Device* GetDevice() {
		return device;
	}
	inline IDXGISwapChain* GetSwapChain() {
		return swapChain;
	}
	inline ID3D11RenderTargetView* GetRenderTargetView() {
		return RTV;
	}
	inline ID3D11DepthStencilView* GetDepthStencilView() {
		return DSV;
	}
	inline ID3D11DeviceContext* GetDeviceContext() {
		return deviceContext;
	}
	inline ID3D11ShaderResourceView* GetShaderResourceView() {
		return SRV;
	}
	inline ID3D11SamplerState* GetSamplerState() {
		return samplerState;
	}
	inline D3D11_VIEWPORT GetViewport() {
		return viewport;
	}
	// ================= //

};

