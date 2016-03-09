#pragma once
#include "Object.h"

#include <vector>

class ObjectManager
{
private:
	// === Static
	static ObjectManager* s_Instance;

	// === Member
	std::vector<Object*> m_Objects;

	// === Private Interface === //
	ObjectManager() { }
	ObjectManager(const ObjectManager &) { }
	ObjectManager(const ObjectManager &&) { }
	ObjectManager& operator=(const ObjectManager &) { }
	ObjectManager& operator=(const ObjectManager &&) { }

	void Initialize();
	// ========================= //

public:
	// === Destructor === //
	~ObjectManager();
	// ================== //

	// === Static Interface === //
	static ObjectManager* GetInstance();
	// ======================== //

	// === Interface === //
	Object* CreateNewObject();
	void UpdateObjects();
	void Terminate();
	// ================= //
};

