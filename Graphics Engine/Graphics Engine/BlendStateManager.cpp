///////////////////////////////////////////////////////////////////
// BlendStateManager.cpp
//
// Created By:		Doug Berg
// Last Updated:	3.2.2016
///////////////////////////////////////////////////////////////////
#include "BlendStateManager.h"

#include "Renderer.h"

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

//////////////////////////////////////////////////
// SINGLETON
//////////////////////////////////////////////////
/*static*/ BlendStateManager* BlendStateManager::s_Instance = nullptr;

/*static*/ BlendStateManager* BlendStateManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new BlendStateManager;
		s_Instance->Initialize();
	}
	return s_Instance;
}

void BlendStateManager::DeleteInstance()
{
	delete s_Instance;
	s_Instance = nullptr;
}
//////////////////////////////////////////////////





//////////////////////////////////////////////////
// INTERFACE
//////////////////////////////////////////////////
void BlendStateManager::Initialize()
{
	// Initialize ID3D11BlendState's here...
	D3D11_BLEND_DESC blendDesc;

	// === Default BlendState
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Renderer::GetInstance()->GetDevice()->CreateBlendState(&blendDesc, &m_blendStates[BlendState_Default]);
}

void BlendStateManager::Terminate()
{
	for (int i = 0; i < BlendStates::MAX_BLENDSTATES; ++i) {
		SAFE_RELEASE(m_blendStates[i]);
	}

	delete s_Instance;
}

void BlendStateManager::Revert()
{
	Renderer::GetInstance()->GetDeviceContext()->OMSetBlendState(m_blendStates[BlendState_Default], 0, 0xFFFFFFFF);
}

bool BlendStateManager::Apply(BlendStates _state)
{
	if (_state < 0 || _state >= MAX_BLENDSTATES)
		return false;

	Renderer::GetInstance()->GetDeviceContext()->OMSetBlendState(m_blendStates[_state], 0, 0xFFFFFFFF);

	return true;
}
//////////////////////////////////////////////////

