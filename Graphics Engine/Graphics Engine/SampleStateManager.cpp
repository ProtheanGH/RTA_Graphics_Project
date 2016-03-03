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
	Renderer::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &m_SampleStates[Default]);
}

void SampleStateManager::Shutdown()
{
	// === Release all the SampleStates
	for (int i = 0; i < MAX_SAMPLE_STATES; ++i) {
		SAFE_RELEASE(m_SampleStates[i]);
	}
}
// ===================== //

// ===== Private Interface ===== //
void SampleStateManager::Initialize()
{
	// *** Add ALL Sample States here
}
// ============================= //
