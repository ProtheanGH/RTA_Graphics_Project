#pragma once
#include "RenderNode.h"
#include "RenderSet.h"

#include <d3d11.h>

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

class RenderMaterial : public RenderNode
{
private:
	// === Members
	ID3D11Resource* m_pTexture;
	RenderSet m_RShapesSet;

	// === Private Interface === //
	void Apply();
	void Revert();
	void DefaultMaterial_RenderProcess(RenderNode& _node);
	// ========================= //

public:
	// === Constructor / Destructor === //
	RenderMaterial();
	virtual ~RenderMaterial();
	// ================================ //

	// === Interface === //
	virtual void Add(RenderNode* _rMaterial, RenderNode* _rShape);
	// ================= //
};

