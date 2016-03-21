#pragma once
#include "RenderNode.h"
#include "Object.h"

#include <d3d11.h>

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

class RenderShape : public RenderNode
{
	// === Members
	Object*						m_pObject;
	D3D11_PRIMITIVE_TOPOLOGY	m_TopologyType;

	// === Private Interface === //
	static void DefaultShape_RenderProcess(RenderNode& _node);
	static void Skybox_RenderProcess(RenderNode& _node);
	static void AnimatedShape_RenderProcess(RenderNode& _node);
	// ========================= //

public:
	// === Constructor / Destructor === //
	RenderShape();
	virtual ~RenderShape();
	// ================================ //

	// === Accessors === //
	inline Object* GetShapeObject() {
		return m_pObject;
	}
	// ================= //

	// === Mutators === //
	inline void SetObject(Object* _object) {
		m_pObject = _object;
	}
	inline void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) {
		m_TopologyType = _topology;
	}
	// ================ //
};

