#pragma once

#include "BlendStateManager.h"
#include "RasterizerStateManager.h"
#include "RenderNodeDirectory.h"
#include "SampleStateManager.h"
#include "ShaderManager.h"
#include "VertexFormats.h"

#include <DirectXMath.h>
#include <vector>

using DirectX::XMFLOAT3;

class GameObject
{
public:
	typedef void(*RenderFunc)(RenderNode& _Node);

protected:
	// === Render Context Settings
	PixelShaderEnum			 m_PixelShader	   = PixelShaderEnum::Pixel_Default;
	VertexShaderEnum		 m_VertexShader	   = VertexShaderEnum::Vertex_Default;
	BlendStates				 m_BlendState	   = BlendStates::BlendState_Default;
	RasterizerStates		 m_RasterizerState = RasterizerStates::RasterizerState_Default;
	InputLayouts			 m_InputLayout	   = InputLayouts::InputLayout_Default;

	// === Render Material Settings
	SampleStates			 m_SampleState	   = SampleStates::SampleState_Default;
	std::vector<std::string> m_TextureIDs;

	// === Render Shape Settings
	D3D11_PRIMITIVE_TOPOLOGY m_TopologyType    = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	std::string				 m_MeshPath;
	RenderFunc				 m_RenderFunc	   = RenderShape::DefaultShape_RenderProcess;

	// === Object Settings
	XMFLOAT3				 m_WorldPosition   = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3				 m_WorldRotation   = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3				 m_WorldScale      = XMFLOAT3(1.0f, 1.0f, 1.0f);

	// === Private Interface === //
	virtual void AddToRenderer(RenderContext* _context, RenderMaterial* _material, RenderShape* _shape);
	virtual void LoadComponents(Object* _object);
	// ========================= //

public:
	// === Constructor / Destructor === //
	GameObject();
	virtual ~GameObject();
	// ================================ //

	// === Interface === //
	void LoadGameObject();
	// ================= //

	// === Mutators === //
	inline void SetPixelShader(PixelShaderEnum _type) {
		m_PixelShader = _type;
	}
	inline void SetVertexShader(VertexShaderEnum _type) {
		m_VertexShader = _type;
	}
	inline void SetBlendState(BlendStates _type) {
		m_BlendState = _type;
	}
	inline void SetRasterizerState(RasterizerStates _type) {
		m_RasterizerState = _type;
	}
	inline void SetInputLayout(InputLayouts _type) {
		m_InputLayout = _type;
	}
	inline void SetSampleState(SampleStates _type) {
		m_SampleState = _type;
	}
	inline void SetTextureIDs(std::vector<std::string> _paths) {
		m_TextureIDs = _paths;
	}
	inline void SetMeshPath(std::string _path) {
		m_MeshPath = _path;
	}
	inline void SetWorldPosition(XMFLOAT3 _position) {
		m_WorldPosition = _position;
	}
	inline void SetWorldRotation(XMFLOAT3 _rotation) {
		m_WorldRotation = _rotation;
	}
	inline void SetWorldScale(XMFLOAT3 _scale) {
		m_WorldScale = _scale;
	}
	// ================= //
};

