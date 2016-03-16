#pragma once
#include "RenderNode.h"
#include "RenderSet.h"
#include "SampleStateManager.h"
#include "ShaderResourceManager.h"

#include <d3d11.h>
#include <vector>

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

class RenderMaterial : public RenderNode
{
private:
	// === Members
	SampleStates m_SampleState;
	std::vector<std::string> m_ShaderResourceIDs;
	RenderSet m_RShapesSet;

	// === Private Interface === //
	void Apply();
	void Revert();

	static void DefaultMaterial_RenderProcess(RenderNode& _node);
	// ========================= //

public:
	// === Constructor / Destructor === //
	RenderMaterial();
	virtual ~RenderMaterial();
	// ================================ //

	// === Interface === //
	virtual void Add(RenderNode* _rMaterial, RenderNode* _rShape);
	virtual bool Compare(RenderNode* _otherNode);
	// ================= //

	// === Mutators === //
	inline void SetSettings(SampleStates _sampleState, std::vector<std::string> _shaderResourceIDs) {
		m_SampleState = _sampleState;
		m_ShaderResourceIDs = _shaderResourceIDs;
	}
	inline void SetSampleState(SampleStates _sampleState) {
		m_SampleState = _sampleState;
	}
	inline void AddShaderResourceID(std::string _shaderResourceID) {
		m_ShaderResourceIDs.push_back(_shaderResourceID);
	}
	// ================ //
};

