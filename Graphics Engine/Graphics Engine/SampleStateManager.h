#pragma once
#include <d3d11.h>

// === Enum
enum SampleStates { SampleState_Default = 0, MAX_SAMPLE_STATES };

class SampleStateManager
{
private:
	// === Static Members
	static SampleStateManager* s_Instance;

	// === Members
	ID3D11SamplerState* m_SampleStates[MAX_SAMPLE_STATES];

	// === Private Interface === //
	SampleStateManager() { }
	SampleStateManager(const SampleStateManager &) { }
	SampleStateManager(const SampleStateManager &&) { }
	SampleStateManager& operator=(const SampleStateManager &) { }
	SampleStateManager& operator=(const SampleStateManager &&) { }

	void Initialize();
	// ========================= //

public:
	// === Destructor === //
	~SampleStateManager();
	// ================== //

	// === Static Interface === //
	static SampleStateManager* GetInstance();
	// ======================== //

	// === Interface === //
	void Apply(SampleStates _sampleState);
	void Revert();
	void Terminate();
	// ================= //
};

