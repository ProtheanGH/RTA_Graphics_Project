#pragma once
#include <d3d11.h>

// === Enums
enum RasterizerStates { Default = 0, MAX_RASTERIZER_STATES };

class RasterizerStateManager
{
private:
	// === Static Members
	static RasterizerStateManager* s_Instance;

	// === Members
	ID3D11RasterizerState* m_RasterizerStates[RasterizerStates::MAX_RASTERIZER_STATES];

	// === Private Interface === //
	RasterizerStateManager() { }
	RasterizerStateManager(const RasterizerStateManager &) { }
	RasterizerStateManager(const RasterizerStateManager &&) { }
	RasterizerStateManager& operator=(const RasterizerStateManager &) { }
	RasterizerStateManager& operator=(const RasterizerStateManager &&) { }

	void Initialize();
	// ========================= //

public:
	// === Destructor === //
	~RasterizerStateManager();
	// ================== //

	// === Static Interface === //
	static RasterizerStateManager* GetInstance();
	// ======================== //

	// === Interface === //
	void Apply(RasterizerStates _rasterizerState);
	void Revert();
	// ================= //
};

