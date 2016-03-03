#include "Application.h"

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 780

// ===== Constructor / Destructor ===== //
Application::Application(HINSTANCE _hinst, WNDPROC _proc)
{
	m_Instance = _hinst;
	m_WindProc = _proc;

	// === Create the Window
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = m_WindProc;
	wndClass.lpszClassName = L"RTA Project";
	wndClass.hInstance = m_Instance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);

	RegisterClassEx(&wndClass);

	RECT windowSize = { 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT };
	AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, false);

	m_Window = CreateWindow(L"RTA Project", L"RTA Project", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
							windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, 
							NULL, NULL, m_Instance, this);

	ShowWindow(m_Window, SW_SHOW);
}

Application::~Application()
{

}
// ==================================== //

// ===== Interface ===== //
bool Application::Run() 
{
	return true;
}
// ===================== //
