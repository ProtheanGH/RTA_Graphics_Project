#pragma once
#include "BlendStateManager.h"
#include "RenderNode.h"
#include "RenderSet.h"
#include "RasterizerStateManager.h"
#include "ShaderManager.h"
#include "VertexFormats.h"

#include <d3d11.h>

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

class RenderContext : public RenderNode
{
private: 
	// === Members
	VertexShaderEnum			m_VertexShader;
	PixelShaderEnum				m_PixelShader;
	BlendStates					m_BlendState;
	RasterizerStates			m_RasterizerStateType;
	InputLayouts				m_InputLayout;
	RenderSet					m_RMaterialSet;

	// === Private Interface === //
	void Apply();
	void Revert();

	static void DefaultContext_RenderProcess(RenderNode& _node);
	// ========================= //

public:
	// === Constructor / Destructor === //
	RenderContext();
	virtual ~RenderContext();
	// ================================ //

	// === Interface === //
	virtual void Add(RenderNode* _rMaterial, RenderNode* _rShape);
	virtual bool Compare(RenderNode* _otherNode);
	// ================= //

	// === Mutators === //
	inline void SetSetting(VertexShaderEnum _vertexShader = VertexShaderEnum::Vertex_Default, PixelShaderEnum _pixelShader = PixelShaderEnum::Pixel_Default, BlendStates _blendState = BlendStates::BlendState_Default, RasterizerStates _rasterizerState = RasterizerStates::RasterizerState_Default, InputLayouts _inputLayout = InputLayouts::InputLayout_Default) {
		m_VertexShader = _vertexShader;
		m_PixelShader = _pixelShader;
		m_BlendState = _blendState;
		m_RasterizerStateType = _rasterizerState;
		m_InputLayout = _inputLayout;
	}
	inline void SetVertexShader(VertexShaderEnum _vertexShader) {
		m_VertexShader = _vertexShader;
	}
	inline void SetPixelShader(PixelShaderEnum _pixelShader) {
		m_PixelShader = _pixelShader;
	}
	inline void SetBlendState(BlendStates _blendState) {
		m_BlendState = _blendState;
	}
	inline void SetRasterizerState(RasterizerStates _rasterizerState) {
		m_RasterizerStateType = _rasterizerState;
	}
	inline void SetInputLayout(InputLayouts _inputLayout) {
		m_InputLayout = _inputLayout;
	}
	// ================ //
};

