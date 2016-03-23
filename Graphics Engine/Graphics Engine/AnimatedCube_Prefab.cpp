#include "AnimatedCube_Prefab.h"


AnimatedCube_Prefab::AnimatedCube_Prefab()
{
	// === RenderContext Settings
	m_PixelShader = PixelShaderEnum::NormalMap_Pixel;
	m_VertexShader = VertexShaderEnum::Animation_Vertex;
	m_InputLayout = InputLayouts::InputLayout_Animation;

	/*m_VertexShader = VertexShaderEnum::NormalMap_Vertex;
	m_InputLayout = InputLayouts::NormalMapped_InputLayout;*/

	// === RenderMaterial Settings
	ShaderResourceManager* resourceManager = ShaderResourceManager::GetInstance();
	m_TextureIDs.push_back(resourceManager->LoadTextureFromFile("Assets/TestCube.dds"));
	m_TextureIDs.push_back(resourceManager->LoadTextureFromFile("Assets/TestCube_Normal.dds"));

	// === RenderShape Settings
	m_MeshPath = "Box_Attack";
	m_RenderFunc = RenderShape::AnimatedShape_RenderProcess;
}


AnimatedCube_Prefab::~AnimatedCube_Prefab()
{
}

// ===== Private Interface ===== //
void AnimatedCube_Prefab::LoadComponents(Object* _object)
{
	_object->AddInterpolator();
	InterpolatorComponent* component = _object->GetInterpolator();
	_object->GetInterpolator()->SetAnimation(_object->GetAnimations()[0]);
}
// ============================= //
