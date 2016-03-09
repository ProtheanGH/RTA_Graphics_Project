#pragma once
//////////////////////////////////////////////////////////////////////////////////
// Created By:		Doug Berg
// Last Updated:	3.7.2016
//////////////////////////////////////////////////////////////////////////////////
#include "RenderSet.h"
#include "RenderContext.h"
#include "RenderMaterial.h"
#include "RenderShape.h"

#include <DirectXMath.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

using namespace DirectX;

class Renderer
{
private:
	// === Static Members
	static Renderer* s_Instance;

	// === DirectX Members
	ID3D11Device             *device;
	IDXGISwapChain           *swapChain;
	ID3D11RenderTargetView   *RTV;
	ID3D11DepthStencilView   *DSV;
	ID3D11Texture2D			 *depthStencil;
	ID3D11DeviceContext      *deviceContext;
	D3D11_VIEWPORT            viewport;
	D3D11_INPUT_ELEMENT_DESC  inputElementDesc;
	XMFLOAT4X4                m_ViewMatrix;
	XMFLOAT4X4                m_ProjectionMatrix;

	// === Members
	RenderSet m_RenderSet;

	// === Private Interface === //
	XMFLOAT4X4 CreateProjectionMatrix(float _fov, float _width, float _height);
	void CreateViewMatrix();
	// ========================= //

	// === DirectX Initialization === //
	void CreateDeviceAndSwapChain ( HWND _window );
	void CreateRTV ( void );
	void CreateDSV(int _width, int _height);
	void SetupViewports(float _width, float _height);
	void CreateInputElementDescription ( void );
	// ============================== //

public:
	// === Constructor / Destructor === //
	Renderer  ( void ) = default;
	~Renderer ( void ) = default;
	Renderer(const Renderer& _copy)                = delete;
	Renderer& operator = (const Renderer& _assign) = delete;
	// ================================ //

	// === SINGLETON CLASS === //
	static Renderer* GetInstance ( void );
	// ======================= //

	// === Interface === //
	void Initialize(HWND _window, const int _samplerCount, const float _screenHeight, const float _screenWidth);
	void Terminate();
	void Render();
	void AddForRendering(RenderContext* _rContext, RenderMaterial* _rMaterial, RenderShape* _rShape);
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
	inline D3D11_VIEWPORT GetViewport() {
		return viewport;
	}
	inline XMFLOAT4X4 GetViewMatrix() const {
		return m_ViewMatrix;
	}
	inline XMFLOAT4X4 GetProjectionMatrix() const {
		return m_ProjectionMatrix;
	}
	// ================= //

	// === Mutators === //
	inline void SetViewMatrix(const XMFLOAT4X4& _viewMatrix) {
		m_ViewMatrix = _viewMatrix;
	}
	inline void SetProjectionMatrix(const XMFLOAT4X4& _projMatrix) {
		m_ProjectionMatrix = _projMatrix;
	}
	// ================ //

};

