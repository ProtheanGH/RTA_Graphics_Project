#pragma once
#include <d3d11.h>

#define VERTEX_FORMATS_MAX 1

// === Enumerations
enum VertexFormat { default };

class VertexFormats
{
public:
	VertexFormats();
	~VertexFormats();
};

class InputLayoutManager
{
private:
	// === Static Members
	static InputLayoutManager* s_Instance;

	// === Members
	ID3D11InputLayout* m_InputLayouts[VERTEX_FORMATS_MAX];

	// === Private Interface === //
	InputLayoutManager() { }
	InputLayoutManager(const InputLayoutManager &) { }
	InputLayoutManager(const InputLayoutManager &&) { }
	InputLayoutManager& operator=(const InputLayoutManager &) { }
	InputLayoutManager& operator=(const InputLayoutManager &&) { }

	void Initialize();
	// ========================= //

public:
	// === Destructor === //
	~InputLayoutManager();
	// ================== //

	// === Static Interface === //
	static InputLayoutManager* GetInstance();
	// ======================== //

	// === Interface === //
	inline ID3D11InputLayout* GetInputLayout(VertexFormat _index) {
		return m_InputLayouts[_index];
	}
	// ================= //
};

