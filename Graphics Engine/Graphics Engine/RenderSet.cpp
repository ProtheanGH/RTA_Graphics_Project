#include "RenderSet.h"

// ===== Constructor / Destructor ===== //
RenderSet::RenderSet()
{

}

RenderSet::~RenderSet()
{

}
// ==================================== //

// ===== Interface ===== //
void RenderSet::Add(RenderNode* _node)
{
	if (m_Head == nullptr) {
		m_Head = _node;
	}
	else {
		_node->m_Next = m_Head;
		m_Head = _node;
	}
	++m_Count;
}

bool RenderSet::Remove(RenderNode* _node)
{
	RenderNode *curr = m_Head, *prev = nullptr;

	while (curr != nullptr) {
		if (curr == _node) {
			if (prev == nullptr) {
				// === The Desired Node is the Head
				m_Head = curr->m_Next;
			}
			else {
				// === Normal Node
				prev->m_Next = curr->m_Next;
			}
			--m_Count;
			return true;
		}

		prev = curr;
		curr = curr->m_Next;
	}

	return false;
}
bool RenderSet::RemoveAt(int _index)
{
	RenderNode *curr = m_Head, *prev = nullptr;
	int index = 0;

	while (curr != nullptr) {
		if (index == _index) {
			if (prev == nullptr) {
				// === The Desired Node is the Head
				m_Head = curr->m_Next;
			}
			else {
				// === Normal Node
				prev->m_Next = curr->m_Next;
			}
			--m_Count;
			return true;
		}

		prev = curr;
		curr = curr->m_Next;
		++index;
	}

	return false;
}

void RenderSet::Clear()
{
	m_Head = nullptr;
}

bool RenderSet::Contains(RenderNode* _node)
{
	RenderNode* curr = m_Head;
	while (curr != nullptr) {
		if (curr == _node) {
			return true;
		}
		curr = curr->m_Next;
	}

	return false;
}

RenderNode* RenderSet::Find(RenderNode* _node)
{
	RenderNode* curr = m_Head;
	while (curr != nullptr) {
		if (curr == _node) {
			return curr;
		}
		curr = curr->m_Next;
	}

	return nullptr;
}
// ===================== //
