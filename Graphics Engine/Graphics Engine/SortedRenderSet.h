#pragma once
#include "RenderSet.h"

// Should return true if the _node should be sorted in front of the _compareNode
typedef bool (*CompareFunc)(RenderNode* _node, RenderNode* _compareNode);

class SortedRenderSet : private RenderSet
{
private:
	// === Members
	CompareFunc m_InsertFunc;

public:
	// === Constructor / Destructor === //
	SortedRenderSet(CompareFunc _func);
	~SortedRenderSet();
	// ================================ //

	// === Overloaded Interface === //
	void Add(RenderNode* _node);
	// ============================ //
};

