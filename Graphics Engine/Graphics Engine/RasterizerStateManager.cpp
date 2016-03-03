#include "RasterizerStateManager.h"
#include "Renderer.h"

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
	Renderer::GetInstance()->GetDeviceContext()->RSSetState(m_RasterizerStates[RasterizerStates::Default]);
}
// ===================== //

// === Private Interface === //
void RasterizerStateManager::Initialize()
{
	// *** Add all the Rasterizer States
}
// ========================= //
