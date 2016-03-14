#include "ShaderResourceManager.h"

#include "DDSTextureLoader.h"
#include "Renderer.h"

#include <fstream>

#define SAFE_RELEASE(p) { if(p) { p->Release(); p = nullptr; } }

ShaderResourceManager* ShaderResourceManager::s_Instance = nullptr;

// ===== Destructor ===== //
ShaderResourceManager::~ShaderResourceManager()
{

}
// ====================== //

// ===== Static Interface ===== //
ShaderResourceManager* ShaderResourceManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new ShaderResourceManager();
		s_Instance->Initialize();
	}

	return s_Instance;
}
// ============================ //

// ===== Interface ===== //
std::string ShaderResourceManager::LoadTextureFromFile(std::string _fromFile)
{
	std::string filename = DropFileExtension(_fromFile);

	unsigned int resourceIndex = ContainsResource(filename);
	if (resourceIndex == -1) {
		ID3D11Device* device = Renderer::GetInstance()->GetDevice();

		size_t length = strlen(_fromFile.c_str()) + 1;
		wchar_t path[50];
		int needed = MultiByteToWideChar(0, 0, _fromFile.c_str(), (int)length + 1, path, (int)length + 1);
		
		ID3D11ShaderResourceView* newResource;
		CreateDDSTextureFromFile(device, path, NULL, &newResource);

		m_ResourceIDs.push_back(filename);
		m_Resources.push_back(newResource);
	}
	
	return filename;
}

void ShaderResourceManager::ApplyShaderResource(std::string _resourceID)
{
	ID3D11ShaderResourceView* resource = nullptr;
	unsigned int resourceIndex = ContainsResource(_resourceID);

	if (resourceIndex != -1) {
		resource = m_Resources[resourceIndex];
		Renderer::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &resource);
	}
}

void ShaderResourceManager::Terminate()
{
	// === Release all the ShaderResources
	unsigned int size = (unsigned int)m_Resources.size();
	for (unsigned int i = 0; i < size; ++i) {
		SAFE_RELEASE(m_Resources[i]);
	}

	delete s_Instance;
	s_Instance = nullptr;
}
// ===================== //

// ===== Private Interface ===== //
void ShaderResourceManager::Initialize()
{
	// *** Load in ALL Textures here
}

void ShaderResourceManager::ConvertFileToDDS(char* _filename)
{
	std::fstream file = std::fstream(_filename, std::ios_base::in | std::ios_base::out);

}

unsigned int ShaderResourceManager::ContainsResource(std::string _resourceID)
{
	unsigned int size = (unsigned int)m_ResourceIDs.size();
	for (unsigned int i = 0; i < size; ++i) {
		if (m_ResourceIDs[i] == _resourceID)
			return i;
	}

	return -1;
}

std::string ShaderResourceManager::DropFileExtension(std::string _filepath)
{
	std::string filename = _filepath;

	unsigned int position = (unsigned int)filename.find_last_of(".");
	filename.erase(position);

	return filename;
}
// ============================= //
