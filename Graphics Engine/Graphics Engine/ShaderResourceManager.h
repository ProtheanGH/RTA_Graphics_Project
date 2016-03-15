#pragma once
#include <d3d11.h>
#include <vector>

class ShaderResourceManager
{
private:
	// === Static Members
	static ShaderResourceManager* s_Instance;

	// === Members
	std::vector<std::string> m_ResourceIDs;
	std::vector<ID3D11ShaderResourceView*> m_Resources;

	// === Private Interface === //
	ShaderResourceManager() { }
	ShaderResourceManager(const ShaderResourceManager &) { }
	ShaderResourceManager(const ShaderResourceManager &&) { }
	ShaderResourceManager& operator=(const ShaderResourceManager &) { }
	ShaderResourceManager& operator=(const ShaderResourceManager &&) { }

	void Initialize();
	void ConvertFileToDDS(char* _filename);
	unsigned int ContainsResource(std::string _resourceID);
	std::string DropFileExtension(std::string _filename);
	// ========================= //

public:
	// === Destructor === //
	~ShaderResourceManager();
	// ================== //

	// === Static Interface === //
	static ShaderResourceManager* GetInstance();
	// ======================== //

	// === Interface === //
	std::string LoadTextureFromFile(std::string _fromFile);
	void ApplyShaderResource(std::string _resourceID, UINT _slot);
	void Terminate();
	// ================= // 
};

