#include "ConstantBufferManager.h"

#include "Renderer.h"

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

ConstantBufferManager* ConstantBufferManager::s_Instance = nullptr;

// ===== Destructor ===== //
ConstantBufferManager::~ConstantBufferManager()
{
	
}
// ====================== //

// ===== Static Interface ===== //
ConstantBufferManager* ConstantBufferManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new ConstantBufferManager();
		s_Instance->Initialize();
	}
	
	return s_Instance;
}
// ============================ //

// === Interface === //
void ConstantBufferManager::ApplyObjectBuffer(ToShaderObject* _toShaderObject)
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE subResource;
	deviceContext->Map(m_pObjectCBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &subResource);
	memcpy(subResource.pData, _toShaderObject, sizeof(ToShaderObject));
	deviceContext->Unmap(m_pObjectCBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_pObjectCBuffer);
}

void ConstantBufferManager::ApplySceneBuffer(ToShaderScene* _toShaderScene)
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE subResource;
	deviceContext->Map(m_pSceneCBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &subResource);
	memcpy(subResource.pData, _toShaderScene, sizeof(ToShaderScene));
	deviceContext->Unmap(m_pSceneCBuffer, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_pSceneCBuffer);
}

void ConstantBufferManager::ApplyLightBuffer(ToShaderLight* _toShaderLight)
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE subResource;
	deviceContext->Map(m_pLightsCBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &subResource);
	memcpy(subResource.pData, _toShaderLight, sizeof(ToShaderLight));
	deviceContext->Unmap(m_pLightsCBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pLightsCBuffer);
}

void ConstantBufferManager::ApplySkyboxBuffer(ToShaderSkybox* _toShaderSkybox)
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE subResource;
	deviceContext->Map(m_pSkyboxCBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy(subResource.pData, _toShaderSkybox, sizeof(ToShaderSkybox));
	deviceContext->Unmap(m_pSkyboxCBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_pSkyboxCBuffer);
}

void ConstantBufferManager::ApplyAnimatedBuffer(ToShaderAnimated* _toShaderAnimated)
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE subResource;
	deviceContext->Map(m_pAnimatedCBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	memcpy(subResource.pData, _toShaderAnimated, sizeof(ToShaderAnimated));
	deviceContext->Unmap(m_pAnimatedCBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_pAnimatedCBuffer);
}

void ConstantBufferManager::Terminate()
{
	SAFE_RELEASE(m_pObjectCBuffer);
	SAFE_RELEASE(m_pSceneCBuffer);
	SAFE_RELEASE(m_pLightsCBuffer);
	SAFE_RELEASE(m_pSkyboxCBuffer);
	SAFE_RELEASE(m_pAnimatedCBuffer);

	delete s_Instance;
	s_Instance = nullptr;
}
// ================= //

// ===== Private Interface ===== //
void ConstantBufferManager::Initialize()
{
	D3D11_BUFFER_DESC bufferDesc;
	HRESULT hr;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	// == Object Buffer
	bufferDesc.ByteWidth = sizeof(ToShaderObject);
	hr = Renderer::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pObjectCBuffer);
	assert(hr == S_OK);

	// == Scene Buffer
	bufferDesc.ByteWidth = sizeof(ToShaderScene);
	hr = Renderer::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pSceneCBuffer);
	assert(hr == S_OK);

	// == Light Buffer
	bufferDesc.ByteWidth = sizeof(ToShaderLight);
	hr = Renderer::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_pLightsCBuffer);
	assert(hr == S_OK);

	// == Skybox Buffer
	bufferDesc.ByteWidth = sizeof(ToShaderSkybox);
	hr = Renderer::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_pSkyboxCBuffer);
	assert(hr == S_OK);

	// == Animated Buffer
	bufferDesc.ByteWidth = sizeof(ToShaderAnimated);
	hr = Renderer::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_pAnimatedCBuffer);
	assert(hr == S_OK);
}
// ============================= //