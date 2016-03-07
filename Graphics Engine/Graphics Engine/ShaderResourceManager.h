#pragma once
#include <d3d11.h>
#include <map>

class ShaderResourceManager
{
private:
	// === Static Members
	static ShaderResourceManager* s_Instance;

	// === Members
	std::map<std::string, ID3D11ShaderResourceView*> m_ShaderResources;

	// === Private Interface === //
	ShaderResourceManager() { }
	ShaderResourceManager(const ShaderResourceManager &) { }
	ShaderResourceManager(const ShaderResourceManager &&) { }
	ShaderResourceManager& operator=(const ShaderResourceManager &) { }
	ShaderResourceManager& operator=(const ShaderResourceManager &&) { }

	void Initialize();
	// ========================= //

public:
	// === Destructor === //
	~ShaderResourceManager();
	// ================== //

	// === Static Interface === //
	static ShaderResourceManager* GetInstance();
	// ======================== //

	// === Interface === //
	void Terminate();
	// ================= // 

	// === Accessors === //
	inline ID3D11ShaderResourceView* GetShaderResource(std::string _resourceName) {
		return m_ShaderResources[_resourceName];
	}
	// ================= //
};

