#include "VertexFormats.h"

// === VertexFormats
VertexFormats::VertexFormats()
{
}

VertexFormats::~VertexFormats()
{
}
// ===

// === InputLayoutManager
InputLayoutManager* InputLayoutManager::s_Instance = nullptr;

// === Destructor === //
InputLayoutManager::~InputLayoutManager()
{

}
// ================== //

// === Static Interface === //
InputLayoutManager* InputLayoutManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new InputLayoutManager();
		s_Instance->Initialize();
	}

	return s_Instance;
}
// ======================== //

// === Private Interface === //
void InputLayoutManager::Initialize()
{
	// *** Add in all the Input Layouts
}
// ========================= //

// ===
