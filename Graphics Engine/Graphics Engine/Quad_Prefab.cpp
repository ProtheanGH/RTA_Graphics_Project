#include "Quad_Prefab.h"

// ===== Constructor / Destructor ===== //
Quad_Prefab::Quad_Prefab()
{
	// === Render Material Settings
	m_TextureIDs.push_back(ShaderResourceManager::GetInstance()->LoadTextureFromFile("Assets/CartoonWood_Texture.dds"));

	// === Render Shape Settings
	m_MeshPath = "FloorQuad";
}

Quad_Prefab::~Quad_Prefab()
{

}
// ==================================== //
