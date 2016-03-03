#pragma once
#include <map>

#include "Mesh.h"

class MeshManager
{
private:
	// === Static Members
	static MeshManager* s_Instance;

	// === Members
	std::map<std::string, Mesh*> m_Meshes;

	// === Private Interface === //
	MeshManager() { }
	MeshManager(const MeshManager &) { }
	MeshManager(const MeshManager &&) { }
	MeshManager& operator=(const MeshManager &) { }
	MeshManager& operator=(const MeshManager &&) { }

	void Initialize();
	// ========================= //

public:
	// === Destructor === //
	~MeshManager();
	// ================== //

	// === Static Interface === //
	static MeshManager* GetInstance();
	// ======================== //

	// === Interface === //
	void AddMesh(std::string _ID, Mesh* _mesh);
	void Shutdown();
	// ================= //

	// === Accessors === //
	inline Mesh* GetMesh(std::string _ID) {
		return m_Meshes[_ID];
	}
	// ================= //
};

