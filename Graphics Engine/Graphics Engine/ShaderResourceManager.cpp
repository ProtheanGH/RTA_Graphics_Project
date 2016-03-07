#include "ShaderResourceManager.h"

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

ShaderResourceManager* ShaderResourceManager::s_Instance = nullptr;

// ===== Destructor ===== //
ShaderResourceManager::~ShaderResourceManager()
{

}
// ====================== //

// ===== Static Interface ===== //
ShaderResourceManager* ShaderResourceManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new ShaderResourceManager();
		s_Instance->Initialize();
	}

	return s_Instance;
}
// ============================ //

// ===== Interface ===== //
void ShaderResourceManager::Terminate()
{
	// === Release all the ShaderResources
	for (std::map<std::string, ID3D11ShaderResourceView*>::iterator iter = m_ShaderResources.begin(); iter != m_ShaderResources.end(); ++iter) {
		SAFE_RELEASE(iter->second);
	}

}
// ===================== //

// ===== Private Interface ===== //
void ShaderResourceManager::Initialize()
{
	// *** Load in ALL Textures here
}
// ============================= //
