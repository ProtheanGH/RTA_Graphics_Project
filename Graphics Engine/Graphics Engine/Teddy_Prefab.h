#pragma once
#include "GameObject.h"

class Teddy_Prefab : public GameObject
{
private:
	// === Private Interface === //
	virtual void LoadComponents(Object* _object);
	// ========================= //
public:
	// === Constructor / Destructor === //
	Teddy_Prefab();
	virtual ~Teddy_Prefab();
	// ================================ //
};

