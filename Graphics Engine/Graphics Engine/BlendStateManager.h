///////////////////////////////////////////////////////////////////
// BlendStateManager.h
// 
// Created By:		Doug Berg
// Last Updated:	3.2.2016
///////////////////////////////////////////////////////////////////
#pragma once

// DirectX
#include <DirectXMath.h>
using namespace DirectX;
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

// Include
#include "Renderer.h"


enum BlendStates{ Default = 0, 
	MAX_BLENDSTATES };


class BlendStateManager
{
private:
	// === Static Members
	static BlendStateManager *s_Instance;

	// === Members
	ID3D11BlendState         *m_blendStates[MAX_BLENDSTATES];

public:
	// === Defaults === //
	BlendStateManager ( void ) = default;
	~BlendStateManager( void ) = default;
	BlendStateManager(const BlendStateManager& _copy)                = delete;
	BlendStateManager& operator = (const BlendStateManager& _assign) = delete;
	// ================ //


	// === Singleton === //
	static BlendStateManager* GetInstance( void );
	void DeleteInstance( void );
	// ================= //


	// === Interface === //
	void Initialize( void );
	void Terminate ( void );
	void Revert    ( void );
	bool Apply     ( BlendStates _state );
	// ================= //
};

