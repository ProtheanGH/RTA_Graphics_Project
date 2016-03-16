#pragma once
#include "RenderContext.h"
#include "RenderMaterial.h"
#include "RenderShape.h"

#include <vector>

class RenderNodeDirectory
{
private:
	// === Static Members
	static RenderNodeDirectory* s_Instance;

	// === Members
	std::vector<RenderContext*> m_RenderContextes;
	std::vector<RenderMaterial*> m_RenderMaterials;
	std::vector<RenderShape*> m_RenderShapes;

	// === Private Interface === //
	RenderNodeDirectory() { }
	RenderNodeDirectory(const RenderNodeDirectory &) { }
	RenderNodeDirectory(const RenderNodeDirectory &&) { }
	RenderNodeDirectory& operator=(const RenderNodeDirectory &) { }
	RenderNodeDirectory& operator=(const RenderNodeDirectory &&) { }

	void Initialize();
	int ContainsRenderContext(RenderContext* _context);
	int ContainsRenderMaterial(RenderMaterial* _material);
	// ========================= //

public:
	// === Destructor === //
	~RenderNodeDirectory();
	// ================== //

	// === Static Interface === //
	static RenderNodeDirectory* GetInstance();
	// ======================== //

	// === Interface === //
	RenderContext* CreateRenderContext(VertexShaderEnum _vertexShader = VertexShaderEnum::Vertex_Default, PixelShaderEnum _pixelShader = PixelShaderEnum::Pixel_Default, BlendStates _blendState = BlendStates::BlendState_Default, RasterizerStates _rasterizerState = RasterizerStates::RasterizerState_Default, InputLayouts _inputLayout = InputLayouts::InputLayout_Default);
	RenderMaterial* CreateRenderMaterial(SampleStates _sampleState = SampleStates::SampleState_Default, std::vector<std::string> _Textures = std::vector<std::string>());
	RenderShape* CreateRenderShape();
	void Terminate();
	// ================= //
};

