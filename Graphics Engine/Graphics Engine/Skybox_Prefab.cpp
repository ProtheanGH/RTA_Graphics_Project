#include "Skybox_Prefab.h"

#include "Renderer.h"
#include "SkyboxComponent.h"

// ===== Constructor / Destructor ===== //
Skybox_Prefab::Skybox_Prefab()
{
	// === Render Context Settings
	m_VertexShader = VertexShaderEnum::Skybox_Vertex;
	m_PixelShader = PixelShaderEnum::Skybox_Pixel;
	m_RasterizerState = RasterizerStates::Front_Culling;
	m_InputLayout = InputLayouts::SkyboxMapped_InputLayout;

	// === Render Material Settings
	m_TextureIDs.push_back(ShaderResourceManager::GetInstance()->LoadTextureFromFile("Assets/Skybox_Texture.dds"));

	// === Render Shape Settings
	m_MeshPath = "Cube";
}

Skybox_Prefab::~Skybox_Prefab()
{

}
// =================================== //

// ===== Private Interface ===== //
void Skybox_Prefab::AddToRenderer(RenderContext* _context, RenderMaterial* _material, RenderShape* _shape)
{
	Renderer* renderer = Renderer::GetInstance();

	// === Add the Root Object
	renderer->SetSkybox(_context, _material, _shape);

	// === Add all the Children
	RenderShape* childShape;
	size_t childCount = _shape->GetShapeObject()->GetChildren().size();
	for (size_t i = 0; i < childCount; ++i) {
		childShape = RenderNodeDirectory::GetInstance()->CreateRenderShape();
		childShape->SetObject(_shape->GetShapeObject()->GetChildren()[i]);
		childShape->SetTopology(m_TopologyType);

		_context->Add(_material, childShape);
	}
}

void Skybox_Prefab::LoadComponents(Object* _object)
{
	_object->AddComponent(new SkyboxComponent(_object));
}
// ============================= //
