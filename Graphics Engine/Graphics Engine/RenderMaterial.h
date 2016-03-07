#pragma once
#include "RenderNode.h"
#include "RenderSet.h"
#include "SampleStateManager.h"
#include "ShaderResourceManager.h"

#include <d3d11.h>

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

class RenderMaterial : public RenderNode
{
private:
	// === Members
	SampleStates m_SampleState;
	std::string m_ShaderResourceID;
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

	// === Mutators === //
	inline void SetSettings(SampleStates _sampleState, std::string _shaderResourceID) {
		m_SampleState = _sampleState;
		m_ShaderResourceID = _shaderResourceID;
	}
	inline void SetSampleState(SampleStates _sampleState) {
		m_SampleState = _sampleState;
	}
	inline void SetShaderResourceID(std::string _shaderResourceID) {
		m_ShaderResourceID = _shaderResourceID;
	}
	// ================ //
};

