#pragma once

#include <Windows.h>

class Application
{
private:
	// === Members
	HINSTANCE m_Instance;
	WNDPROC m_WindProc;
	HWND m_Window;

	
public:
	// === Constructor / Destructor
	Application(HINSTANCE _hinst, WNDPROC _proc);
	~Application();
	// ===

	// === Interface
	bool Run();
	// ===
};

