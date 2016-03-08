#include "SampleStateManager.h"

#include "Renderer.h"

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

SampleStateManager* SampleStateManager::s_Instance = nullptr;

// ===== Destructor ===== //
SampleStateManager::~SampleStateManager()
{

}
// ====================== //

// === Static Interface === //
SampleStateManager* SampleStateManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new SampleStateManager();
		s_Instance->Initialize();
	}

	return s_Instance;
}
// ======================== //

// ===== Interface ===== //
void SampleStateManager::Apply(SampleStates _sampleState)
{
	if (_sampleState >= MAX_SAMPLE_STATES)
		return;
	Renderer::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &m_SampleStates[_sampleState]);
}

void SampleStateManager::Revert()
{
	Renderer::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &m_SampleStates[SampleState_Default]);
}

void SampleStateManager::Terminate()
{
	// === Release all the SampleStates
	for (int i = 0; i < MAX_SAMPLE_STATES; ++i) {
		SAFE_RELEASE(m_SampleStates[i]);
	}

	delete s_Instance;
	s_Instance = nullptr;
}
// ===================== //

// ===== Private Interface ===== //
void SampleStateManager::Initialize()
{
	D3D11_SAMPLER_DESC samplerDesc;

	// === Default
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	Renderer::GetInstance()->GetDevice()->CreateSamplerState(&samplerDesc, &m_SampleStates[SampleState_Default]);
}
// ============================= //
