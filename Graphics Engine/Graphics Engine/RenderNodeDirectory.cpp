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
RenderContext* RenderNodeDirectory::CreateRenderContext(VertexShaderEnum _vertexShader, PixelShaderEnum _pixelShader, BlendStates _blendState, RasterizerStates _rasterizerState, InputLayouts _inputLayout)
{
	RenderContext* newRContext = new RenderContext();
	newRContext->SetSetting(_vertexShader, _pixelShader, _blendState, _rasterizerState, _inputLayout);

	int result = ContainsRenderContext(newRContext);
	if (result == -1) {
		m_RenderContextes.push_back(newRContext);
		return newRContext;
	}
	else {
		delete newRContext;
		return m_RenderContextes[result];
	}
	
}

RenderMaterial* RenderNodeDirectory::CreateRenderMaterial(SampleStates _sampleState, std::vector<std::string> _Textures)
{
	RenderMaterial* newRMaterial = new RenderMaterial();
	newRMaterial->SetSettings(_sampleState, _Textures);
	int result = ContainsRenderMaterial(newRMaterial);
	if (result == -1) {
		m_RenderMaterials.push_back(newRMaterial);
		return newRMaterial;
	}
	else {
		delete newRMaterial;
		return m_RenderMaterials[result];
	}
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

// === ContainsRenderContext
//  - Searches through the already existing RenderContextes and compares them to the given rContext. Returns the index of the matching context, otherwise, 
//    returns -1.
int RenderNodeDirectory::ContainsRenderContext(RenderContext* _context)
{
	size_t count = m_RenderContextes.size();
	for (size_t i = 0; i < count; ++i) {
		if (m_RenderContextes[i]->Compare(_context))
			return (int)i;
	}

	return -1;
}

int RenderNodeDirectory::ContainsRenderMaterial(RenderMaterial* _material)
{
	size_t count = m_RenderMaterials.size();
	for (size_t i = 0; i < count; ++i) {
		if (m_RenderMaterials[i]->Compare(_material))
			return (int)i;
	}

	return -1;
}
// ============================= //
