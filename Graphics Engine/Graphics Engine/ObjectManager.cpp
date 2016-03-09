#include "ObjectManager.h"

ObjectManager* ObjectManager::s_Instance = nullptr;

// ===== Destructor ===== //
ObjectManager::~ObjectManager()
{

}
// ====================== //

// ===== Static Interface ===== //
ObjectManager* ObjectManager::GetInstance()
{
	if (s_Instance == nullptr) {
		s_Instance = new ObjectManager();
		s_Instance->Initialize();
	}

	return s_Instance;
}
// ============================ //

// ===== Interface ===== //
Object* ObjectManager::CreateNewObject()
{
	Object* newObject = Object::Create();
	m_Objects.push_back(newObject);
	return newObject;
}

void ObjectManager::UpdateObjects()
{
	unsigned int objCount = (unsigned int)m_Objects.size();
	for (unsigned int i = 0; i < objCount; ++i) {
		// === ADD IN UPDATE CODE
	}
}

void ObjectManager::Terminate()
{
	unsigned int objCount = (unsigned int)m_Objects.size();
	for (unsigned int i = 0; i < objCount; ++i) {
		m_Objects[i]->Destroy();
	}

	delete s_Instance;
	s_Instance = nullptr;
}
// ===================== //

// ===== Private Interface ===== //
void ObjectManager::Initialize()
{

}
// ============================= //
