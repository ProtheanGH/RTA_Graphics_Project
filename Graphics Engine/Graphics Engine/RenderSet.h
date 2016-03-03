#pragma once

#include "RenderNode.h"

class RenderSet
{
private:
	// === Members
	RenderNode* m_Head;
	unsigned int m_Count;
public:
	// === Constructor / Destructor === //
	RenderSet();
	~RenderSet();
	// ================================ //

	// === Interface === //
	void Add(RenderNode* _node);
	bool Remove(RenderNode* _node);
	bool RemoveAt(int _index);
	void Clear();
	bool Contains(RenderNode* _node);
	RenderNode* Find(RenderNode* _node);
	// ================= //

	// === Accessors === //
	inline unsigned int GetCount() {
		return m_Count;
	}
	inline RenderNode* GetFront() {
		return m_Head;
	}
	// ================= //
};

