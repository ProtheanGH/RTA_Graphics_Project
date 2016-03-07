#pragma once
#include "BlendStateManager.h"
#include "RenderNode.h"
#include "RenderSet.h"
#include "RasterizerStateManager.h"

#include <d3d11.h>

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

class RenderContext : public RenderNode
{
private: 
	// === Members
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;
	BlendStates					m_BlendState;
	RasterizerStates			m_RasterizerStateType;
	RenderSet					m_RMaterialSet;

	// === Private Interface === //
	void Apply();
	void Revert();
	void DefaultContext_RenderProcess(RenderNode& _node);
	// ========================= //

public:
	// === Constructor / Destructor === //
	RenderContext();
	virtual ~RenderContext();
	// ================================ //

	// === Interface === //
	virtual void Add(RenderNode* _rMaterial, RenderNode* _rShape);
	// ================= //

	// === Mutators === //
	inline void SetSetting(ID3D11VertexShader* _vertexShader = nullptr, ID3D11PixelShader* _pixelShader = nullptr, BlendStates _blendState = BlendStates::BlendState_Default, RasterizerStates _rasterizerState = RasterizerStates::RasterizerState_Default) {
		m_pVertexShader = _vertexShader;
		m_pPixelShader = _pixelShader;
		m_BlendState = _blendState;
		m_RasterizerStateType = _rasterizerState;
	}
	inline void SetVertexShader(ID3D11VertexShader* _vertexShader) {
		m_pVertexShader = _vertexShader;
	}
	inline void SetPixelShader(ID3D11PixelShader* _pixelShader) {
		m_pPixelShader = _pixelShader;
	}
	inline void SetBlendState(BlendStates _blendState) {
		m_BlendState = _blendState;
	}
	inline void SetRasterizerState(RasterizerStates _rasterizerState) {
		m_RasterizerStateType = _rasterizerState;
	}
	// ================ //
};

