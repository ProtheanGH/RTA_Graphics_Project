//////////////////////////////////////////////////////////////////
// Component.h
// 
// Created By:		Doug Berg
// Last Update:		3.14.2016
//////////////////////////////////////////////////////////////////
#pragma once


class Object;


class Component
{
protected:
	Object* m_parent;

public:
	// Defaults
	Component( Object* _parent = nullptr );
	Component( const Component& _copy );
	Component& operator = (const Component& _assign);
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

