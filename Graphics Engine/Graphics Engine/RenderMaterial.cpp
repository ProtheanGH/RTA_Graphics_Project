#include "RenderMaterial.h"

#include "Renderer.h"

// ===== Constructor / Destructor ===== //
RenderMaterial::RenderMaterial() : RenderNode()
{
	// === Default Everything
	m_SampleState = SampleStates::SampleState_Default;
	m_Func = DefaultMaterial_RenderProcess;
}

RenderMaterial::~RenderMaterial()
{
	
}
// ==================================== //

// ===== Interface ===== //
void RenderMaterial::Add(RenderNode* _rMaterial, RenderNode* _rShape)
{
	m_RShapesSet.Add(_rShape);
}
// ===================== //

// ===== Private Interface ===== //
void RenderMaterial::Apply()
{
	SampleStateManager::GetInstance()->Apply(m_SampleState);
	ShaderResourceManager::GetInstance()->ApplyShaderResource(m_ShaderResourceID);
}

void RenderMaterial::Revert()
{
	SampleStateManager::GetInstance()->Revert();
}

void RenderMaterial::DefaultMaterial_RenderProcess(RenderNode& _node)
{
	RenderMaterial& rMaterial = (RenderMaterial&)_node;
	rMaterial.Apply();

	RenderNode* currNode = rMaterial.m_RShapesSet.GetFront();
	while (currNode != nullptr) {
		currNode->RenderProcess();
		currNode = currNode->m_Next;
	}

	rMaterial.Revert();
}
// ============================= //
