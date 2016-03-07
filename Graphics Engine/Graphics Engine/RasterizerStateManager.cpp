#include "RasterizerStateManager.h"
#include "Renderer.h"

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

RasterizerStateManager* RasterizerStateManager::s_Instance = nullptr;

// ===== Destructor ===== //
RasterizerStateManager::~RasterizerStateManager()
{

}
// ====================== //

// ===== Static Interface ===== //
RasterizerStateManager* RasterizerStateManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new RasterizerStateManager();
		s_Instance->Initialize();
	}

	return s_Instance;
}
// ============================ //

// ===== Interface ===== //
void RasterizerStateManager::Apply(RasterizerStates _rasterizerState)
{
	if (_rasterizerState >= RasterizerStates::MAX_RASTERIZER_STATES)
		return;
	Renderer::GetInstance()->GetDeviceContext()->RSSetState(m_RasterizerStates[_rasterizerState]);
}

void RasterizerStateManager::Revert()
{
	Renderer::GetInstance()->GetDeviceContext()->RSSetState(m_RasterizerStates[RasterizerStates::RasterizerState_Default]);
}

void RasterizerStateManager::Terminate()
{
	for (int i = 0; i < RasterizerStates::MAX_RASTERIZER_STATES; ++i) {
		SAFE_RELEASE(m_RasterizerStates[i]);
	}

	delete s_Instance;
}
// ===================== //

// === Private Interface === //
void RasterizerStateManager::Initialize()
{
	D3D11_RASTERIZER_DESC rasterDesc;

	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.AntialiasedLineEnable = false; // true;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false; // true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// === Default
	rasterDesc.CullMode = D3D11_CULL_BACK;
	Renderer::GetInstance()->GetDevice()->CreateRasterizerState(&rasterDesc, &m_RasterizerStates[RasterizerState_Default]);

	// === Front Culling
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	Renderer::GetInstance()->GetDevice()->CreateRasterizerState(&rasterDesc, &m_RasterizerStates[Front_Culling]);

	// === Cull None
	rasterDesc.CullMode = D3D11_CULL_NONE;
	Renderer::GetInstance()->GetDevice()->CreateRasterizerState(&rasterDesc, &m_RasterizerStates[No_Culling]);
}
// ========================= //
