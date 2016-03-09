#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

#define VERTEX_FORMATS_MAX 1

// === Vertex Enums
enum VertexFormat { default };

class VertexFormats
{
public:
	VertexFormats();
	~VertexFormats();
};

// === Layout Enums
enum InputLayouts { InputLayout_Default, MAX_INPUT_LAYOUTS };

class InputLayoutManager
{
private:
	// === Static Members
	static InputLayoutManager* s_Instance;

	// === Members
	ID3D11InputLayout* m_InputLayouts[MAX_INPUT_LAYOUTS];

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
	void Apply(InputLayouts _inputLayout);
	void Revert();
	void Terminate();
	// ================= //
};

// === ToShaderObjects
struct ToShaderObject {
	XMFLOAT4X4 ObjectWorldMatrix;
};

struct ToShaderScene {
	XMFLOAT4X4 SceneViewMatrix;
	XMFLOAT4X4 SceneProjectionMatrix;
};

