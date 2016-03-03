#pragma once

class RenderNode
{
	typedef void(*RenderFunc)(RenderNode& _Node);

	// === Members
	RenderFunc m_Func;

public:
	// === Public Members
	RenderNode* m_Next;

	// === Constructor / Destructor === //
	RenderNode();
	virtual ~RenderNode();
	// ================================ //

	// === Interface === //
	inline void RenderProcess() { m_Func(*this); }
	virtual void Add(RenderNode* _rMaterial, RenderNode* _rShape);
	// ================= //
};

