#include "VertexFormats.h"

#include "Renderer.h"

#include "DefaultVertex.h"
#include "NormalMapVertex.h"
#include "SkyboxVertex.h"
#include "AnimationVertex.h"

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

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

// ===== Interface ===== //
void InputLayoutManager::Apply(InputLayouts _inputLayout)
{
	if (_inputLayout >= MAX_INPUT_LAYOUTS) {
		Renderer::GetInstance()->GetDeviceContext()->IASetInputLayout(m_InputLayouts[InputLayout_Default]);
	}
	else {
		Renderer::GetInstance()->GetDeviceContext()->IASetInputLayout(m_InputLayouts[_inputLayout]);
	}
}

void InputLayoutManager::Revert()
{
	Renderer::GetInstance()->GetDeviceContext()->IASetInputLayout(m_InputLayouts[InputLayout_Default]);
}

void InputLayoutManager::Terminate()
{
	for (int i = 0; i < MAX_INPUT_LAYOUTS; ++i) {
		SAFE_RELEASE(m_InputLayouts[i]);
	}

	delete s_Instance;
	s_Instance = nullptr;
}
// ===================== //

// === Private Interface === //
void InputLayoutManager::Initialize()
{
	ID3D11Device* device = Renderer::GetInstance()->GetDevice();
	// === Default InputLayout
	{
		D3D11_INPUT_ELEMENT_DESC layoutDescription[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		device->CreateInputLayout(layoutDescription, sizeof(layoutDescription) / sizeof(D3D11_INPUT_ELEMENT_DESC), DefaultVertex, sizeof(DefaultVertex), &m_InputLayouts[InputLayout_Default]);
	}

	// === NormalMapped InputLayout
	{
		D3D11_INPUT_ELEMENT_DESC layoutDescription[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		device->CreateInputLayout(layoutDescription, sizeof(layoutDescription) / sizeof(D3D11_INPUT_ELEMENT_DESC), NormalMapVertex, sizeof(NormalMapVertex), &m_InputLayouts[NormalMapped_InputLayout]);
	}

	// === Skybox InputLayout
	{
		D3D11_INPUT_ELEMENT_DESC layoutDescription[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMALS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV",       0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		device->CreateInputLayout(layoutDescription, sizeof(layoutDescription) / sizeof(D3D11_INPUT_ELEMENT_DESC), SkyboxVertex, sizeof(SkyboxVertex), &m_InputLayouts[SkyboxMapped_InputLayout]);
	}

	// === Animation InputLayout
	{
		D3D11_INPUT_ELEMENT_DESC layoutDescription[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMALS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV",		  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHT",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "INDICE",   0, DXGI_FORMAT_R32G32B32A32_SINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
			{ "CP_PADDING", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		device->CreateInputLayout(layoutDescription, sizeof(layoutDescription) / sizeof(D3D11_INPUT_ELEMENT_DESC), AnimationVertex, sizeof(AnimationVertex), &m_InputLayouts[InputLayout_Animation]);
	}
}
// ========================= //

// ===
