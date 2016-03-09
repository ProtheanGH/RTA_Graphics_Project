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
	// ========================= //

public:
	// === Destructor === //
	~RenderNodeDirectory();
	// ================== //

	// === Static Interface === //
	static RenderNodeDirectory* GetInstance();
	// ======================== //

	// === Interface === //
	RenderContext* CreateRenderContext();
	RenderMaterial* CreateRenderMaterial();
	// ================= //
};

