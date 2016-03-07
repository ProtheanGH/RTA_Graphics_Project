#include "RenderMaterial.h"

#include "Renderer.h"

// ===== Constructor / Destructor ===== //
RenderMaterial::RenderMaterial() : RenderNode()
{
	m_SampleState = SampleStates::Default;
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
	ID3D11ShaderResourceView* resource = ShaderResourceManager::GetInstance()->GetShaderResource(m_ShaderResourceID);
	Renderer::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &resource);
}

void RenderMaterial::Revert()
{
	SampleStateManager::GetInstance()->Revert();
}

void RenderMaterial::DefaultMaterial_RenderProcess(RenderNode& _node)
{
	Apply();

	RenderNode* currNode = m_RShapesSet.GetFront();
	while (currNode != nullptr) {
		currNode->RenderProcess();
		currNode = currNode->m_Next;
	}

	Revert();
}
// ============================= //
