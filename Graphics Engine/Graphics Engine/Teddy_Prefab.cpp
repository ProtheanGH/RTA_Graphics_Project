#include "Teddy_Prefab.h"

// ===== Constructor / Destructor ===== //
Teddy_Prefab::Teddy_Prefab()
{
	// === RenderContext Settings
	m_PixelShader = PixelShaderEnum::NormalMap_Pixel;
	m_VertexShader = VertexShaderEnum::NormalMap_Vertex;
	m_InputLayout = InputLayouts::NormalMapped_InputLayout;

	// === RenderMaterial Settings
	ShaderResourceManager* resourceManager = ShaderResourceManager::GetInstance();
	m_TextureIDs.push_back(resourceManager->LoadTextureFromFile("Assets/Teddy_D.dds"));
	m_TextureIDs.push_back(resourceManager->LoadTextureFromFile("Assets/Teddy_Normal.dds"));

	// === RenderShape Settings
	m_MeshPath = "Teddy_Idle";
}

Teddy_Prefab::~Teddy_Prefab()
{

}
// ==================================== //