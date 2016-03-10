//////////////////////////////////////////////////////////////////
// Component.h
// 
// Created By:		Doug Berg
// Last Update:		3.10.2016
//////////////////////////////////////////////////////////////////
#pragma once


class Object;


class Component
{
protected:
	Object* m_parent;

public:
	// Defaults
	Component( void );
	virtual ~Component( void ) = 0;

	// Accessors
	Object* GetParent( void ) const;

	// Mutators
	void SetParent( Object* _parent );

	// Interface
	virtual bool Update( void );
	virtual void Initialize( Object* _parent );
	virtual void Destroy( void );
};

