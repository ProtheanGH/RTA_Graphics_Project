#include "RenderMaterial.h"

// ===== Constructor / Destructor ===== //
RenderMaterial::RenderMaterial() : RenderNode()
{
	// === Set everything to null
	m_pTexture = nullptr;
}

RenderMaterial::~RenderMaterial()
{
	// === Release all objects
	SAFE_RELEASE(m_pTexture);
}
// ==================================== //

// ===== Interface ===== //
void RenderMaterial::Add(RenderNode* _rMaterial, RenderNode* _rShape)
{

}
// ===================== //

// ===== Private Interface ===== //
void RenderMaterial::Apply()
{

}

void RenderMaterial::Revert()
{

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
