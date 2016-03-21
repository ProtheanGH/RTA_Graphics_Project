#pragma once
#include "GameObject.h"
class Skybox_Prefab :
	public GameObject
{
private:
	// === Private Interface === //
	virtual void AddToRenderer(RenderContext* _context, RenderMaterial* _material, RenderShape* _shape);
	virtual void LoadComponents(Object* _object);
	// ========================= //
public:
	// === Constructor / Destructor === //
	Skybox_Prefab();
	virtual ~Skybox_Prefab();
	// ================================ //
};

