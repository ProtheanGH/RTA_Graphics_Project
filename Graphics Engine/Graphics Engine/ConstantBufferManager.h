#pragma once
#include "VertexFormats.h"

#include <d3d11.h>

class ConstantBufferManager
{
private:
	// === Static Members
	static ConstantBufferManager* s_Instance;

	// === Members
	ID3D11Buffer* m_pObjectCBuffer;
	ID3D11Buffer* m_pSceneCBuffer;
	ID3D11Buffer* m_pLightsCBuffer;
	ID3D11Buffer* m_pSkyboxCBuffer;

	// === Private Interface === //
	ConstantBufferManager() { }
	ConstantBufferManager(const ConstantBufferManager &) { }
	ConstantBufferManager(const ConstantBufferManager &&) { }
	ConstantBufferManager& operator=(const ConstantBufferManager &) { }
	ConstantBufferManager& operator=(const ConstantBufferManager &&) { }

	void Initialize();
	// ========================= //

public:
	// === Destructor === //
	~ConstantBufferManager();
	// ================== //

	// === Static Interface === //
	static ConstantBufferManager* GetInstance();
	// ======================== //

	// === Interface === //
	void ApplyObjectBuffer(ToShaderObject* _toShaderObject);
	void ApplySceneBuffer(ToShaderScene* _toShaderScene);
	void ApplyLightBuffer(ToShaderLight* _toShaderLight);
	void ApplySkyboxBuffer(ToShaderSkybox* _toShaderSkybox);
	void ApplyAnimatedBuffer(ToShaderAnimated* _toShaderAnimated);
	void Terminate();
	// ================= //
};

