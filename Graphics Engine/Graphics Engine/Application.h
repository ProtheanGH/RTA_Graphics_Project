#pragma once
#include "Camera.h"

#include <Windows.h>

class Application
{
private:
	// === Members
	HINSTANCE m_Instance;
	WNDPROC m_WindProc;
	HWND m_Window;
	Camera m_Camera;
	long m_Time;

	// === Private Interface
	void SetupScene();
	// ===
	
public:
	// === Constructor / Destructor
	Application(HINSTANCE _hinst, WNDPROC _proc);
	~Application();
	// ===

	// === Interface
	bool Run();
	// ===
};

