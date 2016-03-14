//////////////////////////////////////////////////////////////////
// Component.cpp
// 
// Created By:		Doug Berg
// Last Update:		3.10.2016
//////////////////////////////////////////////////////////////////
#include "Component.h"
#include "Object.h"



//////////////////////////////////////////////////////////////////
// DEFAULTS
//////////////////////////////////////////////////////////////////
Component::Component()
{
	m_parent = nullptr;
}

/*virtual*/Component::~Component()
{
	Destroy();
}


//////////////////////////////////////////////////////////////////
// ACCESSORS
//////////////////////////////////////////////////////////////////
Object* Component::GetParent() const
{
	return m_parent;
}



//////////////////////////////////////////////////////////////////
// MUTATORS
//////////////////////////////////////////////////////////////////
void Component::SetParent(Object* _parent)
{
	m_parent = _parent;
}



//////////////////////////////////////////////////////////////////
// INTERFACE
//////////////////////////////////////////////////////////////////
/*virtual*/ bool Component::Update()
{
	return true;
}

/*virtual*/ void Component::Initialize(Object* _parent)
{
	SetParent(_parent);
}

/*virtual*/ void Component::Destroy()
{
	if (m_parent != nullptr)
		delete m_parent;
	m_parent = nullptr;
}

