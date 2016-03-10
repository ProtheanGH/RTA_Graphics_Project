//////////////////////////////////////////////////////////////////
// SkyboxComponent.cpp
// 
// Created By:		Doug Berg
// Last Update:		3.10.2016
//////////////////////////////////////////////////////////////////


#include "SkyboxComponent.h"
#include "Object.h"


// Defaults
SkyboxComponent::SkyboxComponent()
{
	ZeroMemory(&m_localMatrix, sizeof(XMFLOAT4X4));
}

SkyboxComponent::~SkyboxComponent()
{
	
}

// Interface
bool SkyboxComponent::Update() // override
{
	if (m_parent != nullptr)
	{
		m_localMatrix = m_parent->GetTransform().GetLocalMatrix();
		return true;
	}
	return false;
}

