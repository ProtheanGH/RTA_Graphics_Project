//////////////////////////////////////////////////////////////////
// SkyboxComponent.h
// 
// Created By:		Doug Berg
// Last Update:		3.14.2016
//////////////////////////////////////////////////////////////////
#pragma once
#include "Camera.h"

#include "Component.h"
#include <DirectXMath.h>
using namespace DirectX;



class SkyboxComponent : public Component
{
private:


public:
	// Defaults
	SkyboxComponent ( Object* _parent = nullptr );
	virtual ~SkyboxComponent( void );

	// Interface
	bool Update( void ) override;
};

