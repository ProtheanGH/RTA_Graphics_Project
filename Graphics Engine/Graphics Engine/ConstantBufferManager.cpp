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

}

void ConstantBufferManager::Terminate()
{
	SAFE_RELEASE(m_pObjectCBuffer);
	SAFE_RELEASE(m_pSceneCBuffer);
	SAFE_RELEASE(m_pLightsCBuffer);

	delete s_Instance;
}
// ================= //

// ===== Private Interface ===== //
void ConstantBufferManager::Initialize()
{
	D3D11_BUFFER_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;

	// == Object Buffer
	bufferDesc.ByteWidth = sizeof(ToShaderObject);
	Renderer::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pObjectCBuffer);

	// == Scene Buffer
	bufferDesc.ByteWidth = sizeof(ToShaderScene);
	Renderer::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pSceneCBuffer);
}
// ============================= //