#pragma once
#include "GameObject.h"
class AnimatedCube_Prefab : public GameObject
{
private:
	// === Private Interface === //
	virtual void LoadComponents(Object* _object);
	// ========================= //
public:
	AnimatedCube_Prefab();
	virtual ~AnimatedCube_Prefab();
};

