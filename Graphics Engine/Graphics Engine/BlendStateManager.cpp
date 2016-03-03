///////////////////////////////////////////////////////////////////
// BlendStateManager.cpp
//
// Created By:		Doug Berg
// Last Updated:	3.2.2016
///////////////////////////////////////////////////////////////////
#include "BlendStateManager.h"

#include "Renderer.h"


//////////////////////////////////////////////////
// SINGLETON
//////////////////////////////////////////////////
/*static*/ BlendStateManager* BlendStateManager::m_instance = nullptr;

/*static*/ BlendStateManager* BlendStateManager::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new BlendStateManager;
	return m_instance;
}

void BlendStateManager::DeleteInstance()
{
	delete m_instance;
	m_instance = nullptr;
}
//////////////////////////////////////////////////





//////////////////////////////////////////////////
// INTERFACE
//////////////////////////////////////////////////
void BlendStateManager::Initialize()
{
	// Initialize ID3D11BlendState's here...
}

void BlendStateManager::Terminate()
{
	
}

void BlendStateManager::Revert()
{
	Renderer::GetInstance()->GetDeviceContext()->OMSetBlendState(m_blendStates[Default], 0, 0xFFFFFFFF);
}

bool BlendStateManager::Apply(BlendStateEnum _state)
{
	if (_state < 0 || _state >= MAX_BLENDSTATES)
		return false;

	Renderer::GetInstance()->GetDeviceContext()->OMSetBlendState(m_blendStates[_state], 0, 0xFFFFFFFF);

	return true;
}
//////////////////////////////////////////////////

