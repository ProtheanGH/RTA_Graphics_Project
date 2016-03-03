#pragma once
#include "RenderNode.h"

#include <d3d11.h>

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

class RenderShape : public RenderNode
{
	// === Members

	// === Private Interface === //
	void DefaultShape_RenderProcess(RenderNode& _node);
	// ========================= //
public:
	// === Constructor / Destructor === //
	RenderShape();
	virtual ~RenderShape();
	// ================================ //
};

