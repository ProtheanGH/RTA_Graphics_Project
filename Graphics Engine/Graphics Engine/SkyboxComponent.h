//////////////////////////////////////////////////////////////////
// SkyboxComponent.h
// 
// Created By:		Doug Berg
// Last Update:		3.10.2016
//////////////////////////////////////////////////////////////////
#pragma once
#include "Component.h"
#include <DirectXMath.h>
using namespace DirectX;



class SkyboxComponent : public Component
{
private:
	XMFLOAT4X4 m_localMatrix;

public:
	// Defaults
	SkyboxComponent ( void );
	~SkyboxComponent( void );

	// Interface
	bool Update( void ) override;
};

