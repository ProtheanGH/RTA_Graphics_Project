#include "TestCube_Prefab.h"

// ===== Constructor / Destructor ===== //
TestCube_Prefab::TestCube_Prefab()
{
	// === Render Context Settings
	m_RasterizerState = RasterizerStates::No_Culling;

	// === RenderMaterial Settings
	ShaderResourceManager* resourceManager = ShaderResourceManager::GetInstance();
	m_TextureIDs.push_back(resourceManager->LoadTextureFromFile("Assets/TestCube.dds"));

	// === RenderShape Settings
	m_MeshPath = "Box";
}

TestCube_Prefab::~TestCube_Prefab()
{

}
// ==================================== //
