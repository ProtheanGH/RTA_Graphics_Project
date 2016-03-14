//////////////////////////////////////////////////////////////////
// SkyboxComponent.cpp
// 
// Created By:		Doug Berg
// Last Update:		3.14.2016
//////////////////////////////////////////////////////////////////


#include "SkyboxComponent.h"
#include "Object.h"
#include "Application.h"


// Defaults
SkyboxComponent::SkyboxComponent()
{

}

SkyboxComponent::~SkyboxComponent()
{
	
}

// Interface
bool SkyboxComponent::Update() // override
{
	if (m_parent != nullptr)
	{
		m_parent->GetTransform().SetLocalMatrix(Application::GetCamera().GetViewMatrix());
		return true;
	}
	return false;
}

