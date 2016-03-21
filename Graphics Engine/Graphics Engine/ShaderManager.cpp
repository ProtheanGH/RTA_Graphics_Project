///////////////////////////////////////////////////////////////////////////
// ShaderManager.cpp
// 
// Author:			Doug Berg
// Last Updated:	3.7.2015
///////////////////////////////////////////////////////////////////////////

#include "ShaderManager.h"

#include "Renderer.h"

// == Shader Files
#include "DefaultPixel.h"
#include "DefaultVertex.h"
#include "NormalMapPixel.h"
#include "NormalMapVertex.h"
#include "SkyboxVertex.h"
#include "SkyboxPixel.h"
#include "AnimationVertex.h"

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

ShaderManager* ShaderManager::s_Instance = nullptr;

// ===== Static Interface ===== //
ShaderManager* ShaderManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new ShaderManager;
		s_Instance->Initialize();
	}

	return s_Instance;
}
// ============================ //

// ===== Interface ===== //
bool ShaderManager::Apply(ShaderTypeEnum _shader, const int _index)
{
	if (_shader < 0 || _shader > SHADER_TYPE_MAX)
		return false;

	switch (_shader)
	{
	case Compute_Shader:
//		Renderer::GetInstance()->GetDeviceContext()->CSSetShader(m_ComputeShaders[_index], nullptr, 0xFFFFFFFF);
		return true;

	case Domain_Shader:
//		Renderer::GetInstance()->GetDeviceContext()->DSSetShader(m_DomainShaders[_index], nullptr, 0xFFFFFFFF);
		return true;

	case Hull_Shader:
//		Renderer::GetInstance()->GetDeviceContext()->HSSetShader(m_HullShaders[_index], nullptr, 0xFFFFFFFF);
		return true;

	case Geometry_Shader:
//		Renderer::GetInstance()->GetDeviceContext()->GSSetShader(m_GeometryShaders[_index], nullptr, 0xFFFFFFFF);
		return true;

	case Pixel_Shader:
		Renderer::GetInstance()->GetDeviceContext()->PSSetShader(m_PixelShaders[_index], nullptr, 0);
		return true;

	case Vertex_Shader:
		Renderer::GetInstance()->GetDeviceContext()->VSSetShader(m_VertexShaders[_index], nullptr, 0);
		return true;
	}
	return false;
}

void ShaderManager::Terminate()
{
//	for (int i = 0; i < COMPUTE_MAX; ++i)
//		SAFE_RELEASE(m_ComputeShaders[i]);
//	for (int i = 0; i < DOMAIN_MAX; ++i)
//		SAFE_RELEASE(m_DomainShaders[i]);
//	for (int i = 0; i < GEOMETRY_MAX; ++i)
//		SAFE_RELEASE(m_GeometryShaders[i]);
//	for (int i = 0; i < HULL_MAX; ++i)
//		SAFE_RELEASE(m_HullShaders[i]);
	for (int i = 0; i < PIXEL_MAX; ++i)
		SAFE_RELEASE(m_PixelShaders[i]);
	for (int i = 0; i < VERTEX_MAX; ++i)
		SAFE_RELEASE(m_VertexShaders[i]);

	delete s_Instance;
	s_Instance = nullptr;
}
// ===================== //

// ===== Private Interface ===== //
void ShaderManager::Initialize()
{
	// === Compute Shaders === //
	// Renderer::GetInstance()->GetDevice()->CreateComputeShader(compiledShader, sizeof(compiledShader), nullptr, &computeShaders[0]);
	// ===


	// === Domain Shaders === //
	// Renderer::GetInstance()->GetDevice()->CreateDomainShader(compiledShader, sizeof(compiledShader), nullptr, &domainShaders[0]);
	// ===


	// === Geometry Shaders === //
	// Renderer::GetInstance()->GetDevice()->CreateGeometryShader(compiledShader, sizeof(compiledShader), nullptr, &geometryShaders[0]);
	// ===


	// === Hull Shaders === //
	// Renderer::GetInstance()->GetDevice()->CreateHullShader(compiledShader, sizeof(compiledShader), nullptr, &hullShaders[0]);
	// ===


	// === Pixel Shaders === //
	Renderer::GetInstance()->GetDevice()->CreatePixelShader(DefaultPixel, sizeof(DefaultPixel), nullptr, &m_PixelShaders[Pixel_Default]);
	Renderer::GetInstance()->GetDevice()->CreatePixelShader(NormalMapPixel, sizeof(NormalMapPixel), nullptr, &m_PixelShaders[NormalMap_Pixel]);
	Renderer::GetInstance()->GetDevice()->CreatePixelShader(SkyboxPixel, sizeof(SkyboxPixel), nullptr, &m_PixelShaders[Skybox_Pixel]);
	// ===


	// === Vertex Shaders === //
	Renderer::GetInstance()->GetDevice()->CreateVertexShader(DefaultVertex,   sizeof(DefaultVertex),   nullptr, &m_VertexShaders[Vertex_Default]);
	Renderer::GetInstance()->GetDevice()->CreateVertexShader(NormalMapVertex, sizeof(NormalMapVertex), nullptr, &m_VertexShaders[NormalMap_Vertex]);
	Renderer::GetInstance()->GetDevice()->CreateVertexShader(SkyboxVertex,    sizeof(SkyboxVertex),    nullptr, &m_VertexShaders[Skybox_Vertex]);
	Renderer::GetInstance()->GetDevice()->CreateVertexShader(AnimationVertex, sizeof(AnimationVertex), nullptr, &m_VertexShaders[Animation_Vertex]);
	// ===
}
// ============================= //

