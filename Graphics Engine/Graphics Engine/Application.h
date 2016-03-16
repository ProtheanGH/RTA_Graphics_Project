#pragma once
#include "Camera.h"

#include <Windows.h>
#include <D3D11SDKLayers.h>
#include "SkyboxComponent.h"

static Camera m_Camera;

class Application
{
private:
	// === Members
	HINSTANCE m_Instance;
	WNDPROC m_WindProc;
	HWND m_Window;
	long m_Time;
	

	// === Private Interface
	void SetupScene();
	void UpdateLighting();
	// ===
	
public:
	// === Constructor / Destructor
	Application(HINSTANCE _hinst, WNDPROC _proc);
	~Application();
	// ===

	// === Interface
	bool Run();
	static Camera GetCamera();
	// ===
};

