#include "RenderNodeDirectory.h"

RenderNodeDirectory* RenderNodeDirectory::s_Instance = nullptr;

// ===== Destructor ===== //
RenderNodeDirectory::~RenderNodeDirectory()
{

}
// ====================== //

// ===== Static Interface ===== //
RenderNodeDirectory* RenderNodeDirectory::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new RenderNodeDirectory();
		s_Instance->Initialize();
	}

	return s_Instance;
}
// ============================ //

// ===== Interface ===== //
RenderContext* RenderNodeDirectory::CreateRenderContext()
{
	RenderContext* newRContext = new RenderContext();
	m_RenderContextes.push_back(newRContext);
	return newRContext;
}

RenderMaterial* RenderNodeDirectory::CreateRenderMaterial()
{
	RenderMaterial* newRMaterial = new RenderMaterial();
	m_RenderMaterials.push_back(newRMaterial);
	return newRMaterial;
}

RenderShape* RenderNodeDirectory::CreateRenderShape()
{
	RenderShape* newRShape = new RenderShape();
	m_RenderShapes.push_back(newRShape);
	return newRShape;
}

void RenderNodeDirectory::Terminate()
{
	unsigned int size = (unsigned int)m_RenderContextes.size();
	for (unsigned int i = 0; i < size; ++i) {
		delete m_RenderContextes[i];
	}

	size = (unsigned int)m_RenderMaterials.size();
	for (unsigned int i = 0; i < size; ++i) {
		delete m_RenderMaterials[i];
	}

	size = (unsigned int)m_RenderShapes.size();
	for (unsigned int i = 0; i < size; ++i) {
		delete m_RenderShapes[i];
	}

	delete s_Instance;
	s_Instance = nullptr;
}
// ===================== //

// ===== Private Interface ===== //
void RenderNodeDirectory::Initialize()
{

}
// ============================= //
