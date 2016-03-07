///////////////////////////////////////////////////////////////////////////
// ShaderManager.cpp
// 
// Author:			Doug Berg
// Last Updated:	3.7.2015
///////////////////////////////////////////////////////////////////////////
#include "ShaderManager.h"

#define SAFE_RELEASE(pointer) {if(pointer){pointer.Release();}}



/*static*/ShaderManager* ShaderManager::m_instance = nullptr;



///////////////////////////////////////////////////////////////////////////
// ACCESSORS
///////////////////////////////////////////////////////////////////////////
ID3D11ComputeShader* ShaderManager::GetComputeShaders(const int _enumValue)
{
	return computeShaders[_enumValue];
}

ID3D11DomainShader* ShaderManager::GetDomainShaders(const int _enumValue)
{
	return domainShaders[_enumValue];
}

ID3D11GeometryShader* ShaderManager::GetGeometryShaders(const int _enumValue)
{
	return geometryShaders[_enumValue];
}

ID3D11HullShader* ShaderManager::GetHullShaders(const int _enumValue)
{
	return hullShaders[_enumValue];
}

ID3D11PixelShader* ShaderManager::GetPixelShaders(const int _enumValue)
{
	return pixelShaders[_enumValue];
}

ID3D11VertexShader* ShaderManager::GetVertexShaders(const int _enumValue)
{
	return vertexShaders[_enumValue];
}





///////////////////////////////////////////////////////////////////////////
// MUTATORS
///////////////////////////////////////////////////////////////////////////
bool ShaderManager::Apply(ShaderTypeEnum _shader, const int _index)
{
	if (_shader < 0 || _shader > SHADER_TYPE_MAX)
		return false;

	switch (_shader)
	{
	case Compute_Shader:
		Renderer::GetInstance()->GetDeviceContext()->CSSetShader(computeShaders[_index], nullptr, 0xFFFFFFFF);
		return true;

	case Domain_Shader:
		Renderer::GetInstance()->GetDeviceContext()->DSSetShader(domainShaders[_index], nullptr, 0xFFFFFFFF);
		return true;

	case Hull_Shader:
		Renderer::GetInstance()->GetDeviceContext()->HSSetShader(hullShaders[_index], nullptr, 0xFFFFFFFF);
		return true;

	case Geometry_Shader:
		Renderer::GetInstance()->GetDeviceContext()->GSSetShader(geometryShaders[_index], nullptr, 0xFFFFFFFF);
		return true;

	case Pixel_Shader:
		Renderer::GetInstance()->GetDeviceContext()->PSSetShader(pixelShaders[_index], nullptr, 0xFFFFFFFF);
		return true;

	case Vertex_Shader:
		Renderer::GetInstance()->GetDeviceContext()->VSSetShader(vertexShaders[_index], nullptr, 0xFFFFFFFF);
		return true;
	}
	return false;
}



///////////////////////////////////////////////////////////////////////////
// SINGLETON
///////////////////////////////////////////////////////////////////////////
ShaderManager* ShaderManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ShaderManager;
		Init();
	}
	return m_instance;
}

void ShaderManager::Terminate()
{
	for (int i = 0; i < COMPUTE_MAX; ++i)
		computeShaders[i]->Release();
	for (int i = 0; i < DOMAIN_MAX; ++i)
		domainShaders[i]->Release();
	for (int i = 0; i < GEOMETRY_MAX; ++i)
		geometryShaders[i]->Release();
	for (int i = 0; i < HULL_MAX; ++i)
		hullShaders[i]->Release();
	for (int i = 0; i < PIXEL_MAX; ++i)
		pixelShaders[i]->Release();
	for (int i = 0; i < VERTEX_MAX; ++i)
		vertexShaders[i]->Release();

	// delete[] computeShaders;
	// delete[] domainShaders;
	// delete[] geometryShaders;
	// delete[] hullShaders;
	// delete[] pixelShaders;
	// delete[] vertexShaders;

	delete m_instance;
	m_instance = nullptr;
}






///////////////////////////////////////////////////////////////////////////
// SINGLETON
///////////////////////////////////////////////////////////////////////////
void ShaderManager::Init()
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
	Renderer::GetInstance()->GetDevice()->CreatePixelShader(DefaultPixel, sizeof(DefaultPixel), nullptr, &pixelShaders[0]);
	// ===


	// === Vertex Shaders === //
	Renderer::GetInstance()->GetDevice()->CreateVertexShader(DefaultVertex, sizeof(DefaultVertex), nullptr, &vertexShaders[0]);
	// ===
}

