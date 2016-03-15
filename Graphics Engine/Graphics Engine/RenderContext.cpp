#include "RenderContext.h"
#include "Renderer.h"

// ===== Constructor / Destructor ===== //
RenderContext::RenderContext() : RenderNode()
{
	// === Default Everything
	m_VertexShader = VertexShaderEnum::Vertex_Default;
	m_PixelShader = PixelShaderEnum::Pixel_Default;
	m_BlendState = BlendStates::BlendState_Default;
	m_RasterizerStateType = RasterizerStates::RasterizerState_Default;
	m_InputLayout = InputLayouts::InputLayout_Default;
	m_Func = DefaultContext_RenderProcess;
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

// === Compare
//  - Compares two RenderNodes, returns tru if they are the same, false otherwise.
bool RenderContext::Compare(RenderNode* _otherNode)
{
	RenderContext* otherContext = dynamic_cast<RenderContext*>(_otherNode);
	if (!otherContext)
		return false;

	if (this->m_BlendState != otherContext->m_BlendState)
		return false;
	if (this->m_Func != otherContext->m_Func)
		return false;
	if (this->m_InputLayout != otherContext->m_InputLayout)
		return false;
	if (this->m_PixelShader != otherContext->m_PixelShader)
		return false;
	if (this->m_RasterizerStateType != otherContext->m_RasterizerStateType)
		return false;
	if (this->m_VertexShader != otherContext->m_VertexShader)
		return false;

	return true;
}
// ===================== //

// ===== Private Interface ===== //
void RenderContext::Apply()
{
	ShaderManager::GetInstance()->Apply(ShaderTypeEnum::Vertex_Shader, m_VertexShader);
	ShaderManager::GetInstance()->Apply(ShaderTypeEnum::Pixel_Shader, m_PixelShader);
	BlendStateManager::GetInstance()->Apply(m_BlendState);
	RasterizerStateManager::GetInstance()->Apply(m_RasterizerStateType);
	InputLayoutManager::GetInstance()->Apply(m_InputLayout);
}

void RenderContext::Revert()
{
	// === Revert to Defaults
	BlendStateManager::GetInstance()->Revert();
	RasterizerStateManager::GetInstance()->Revert();
	InputLayoutManager::GetInstance()->Revert();
}

void RenderContext::DefaultContext_RenderProcess(RenderNode& _node)
{
	RenderContext& rContext = (RenderContext&)_node;

	rContext.Apply();

	RenderNode* currNode = rContext.m_RMaterialSet.GetFront();
	while (currNode != nullptr) {
		currNode->RenderProcess();
		currNode = currNode->m_Next;
	}

	rContext.Revert();
}
// ============================= //
