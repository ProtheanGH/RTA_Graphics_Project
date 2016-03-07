#include <ctime>

#include "Application.h"
#include "FBXConverter.h"

#pragma comment(lib, "d3d11.lib")

Application* pApp;

// === Windows Functions
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	srand(unsigned int(time(0)));
	pApp = new Application(hInstance, (WNDPROC)WndProc);
	MSG msg; ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT && pApp->Run())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete pApp;
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
	switch (message) {
	case (WM_DESTROY) : {
		PostQuitMessage(0);
	} break;
	case (WM_SIZE) : {

	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
// ===