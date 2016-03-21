#include "Teddy_Prefab.h"

#include "Animation.h"
#include "FBXConverter.h"

// ===== Constructor / Destructor ===== //
Teddy_Prefab::Teddy_Prefab()
{
	// === RenderContext Settings
	m_PixelShader = PixelShaderEnum::NormalMap_Pixel;
	m_VertexShader = VertexShaderEnum::Animation_Vertex;
	m_InputLayout = InputLayouts::NormalMapped_InputLayout;

	// === RenderMaterial Settings
	ShaderResourceManager* resourceManager = ShaderResourceManager::GetInstance();
	m_TextureIDs.push_back(resourceManager->LoadTextureFromFile("Assets/Teddy_D.dds"));
	m_TextureIDs.push_back(resourceManager->LoadTextureFromFile("Assets/Teddy_Normal.dds"));

	// === RenderShape Settings
	m_MeshPath = "Teddy_Idle";
	m_RenderFunc = RenderShape::AnimatedShape_RenderProcess;
}

Teddy_Prefab::~Teddy_Prefab()
{

}
// ==================================== //

// ===== Private Interface ===== //
void Teddy_Prefab::LoadComponents(Object* _object)
{
	_object->AddInterpolator();
	_object->GetInterpolator()->SetAnimation(_object->GetAnimations()[0]);
}
// ============================= //