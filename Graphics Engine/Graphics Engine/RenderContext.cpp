#include "RenderContext.h"
#include "Renderer.h"

// ===== Constructor / Destructor ===== //
RenderContext::RenderContext() : RenderNode()
{
	// === Set Everything to Null
	m_VertexShader = VertexShaderEnum::Vertex_Default;
	m_PixelShader = PixelShaderEnum::Pixel_Default;
	m_BlendState = BlendStates::BlendState_Default;
	m_RasterizerStateType = RasterizerStates::RasterizerState_Default;
}

RenderContext::~RenderContext()
{

}
// ==================================== //

// ===== Interface ===== //
void RenderContext::Add(RenderNode* _rMaterial, RenderNode* _rShape)
{
	RenderNode* materialNode = m_RMaterialSet.Find(_rMaterial);
	if (materialNode == nullptr) {
		// === Material hasn't been added yet
		materialNode = _rMaterial;
		m_RMaterialSet.Add(materialNode);
	}

	materialNode->Add(nullptr, _rShape);
}
// ===================== //

// ===== Private Interface ===== //
void RenderContext::Apply()
{
	ShaderManager::GetInstance()->Apply(ShaderTypeEnum::Vertex_Shader, m_VertexShader);
	ShaderManager::GetInstance()->Apply(ShaderTypeEnum::Pixel_Shader, m_PixelShader);
	BlendStateManager::GetInstance()->Apply(m_BlendState);
	RasterizerStateManager::GetInstance()->Apply(m_RasterizerStateType);
}

void RenderContext::Revert()
{
	// === Revert to Defaults
	BlendStateManager::GetInstance()->Revert();
	RasterizerStateManager::GetInstance()->Revert();
}

void RenderContext::DefaultContext_RenderProcess(RenderNode& _node)
{
	Apply();

	RenderNode* currNode = m_RMaterialSet.GetFront();
	while (currNode != nullptr) {
		currNode->RenderProcess();
		currNode = currNode->m_Next;
	}

	Revert();
}
// ============================= //
