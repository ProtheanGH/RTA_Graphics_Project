#include "MeshManager.h"

MeshManager* MeshManager::s_Instance = nullptr;

// ===== Destructor ===== //
MeshManager::~MeshManager()
{

}
// ====================== //

// ===== Static Interface ===== //
MeshManager* MeshManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new MeshManager();
		s_Instance->Initialize();
	}

	return s_Instance;
}
// ============================ //

// ===== Interface ===== //
void MeshManager::AddMesh(std::string _ID, Mesh* _mesh)
{
	m_Meshes.insert(std::pair<std::string, Mesh*>(_ID, _mesh));
}

void MeshManager::Shutdown()
{
	// === Delete all the Meshes
	for (std::map<std::string, Mesh*>::iterator iter = m_Meshes.begin(); iter != m_Meshes.end(); ++iter) {
		delete (iter->second);
	}
}
// ===================== //

// ===== Private Interface ===== //
void MeshManager::Initialize()
{
	
}
// ============================= //
