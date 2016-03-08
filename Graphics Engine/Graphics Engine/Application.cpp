#include "Application.h"

#include "Renderer.h"
#include "BlendStateManager.h"
#include "ConstantBufferManager.h"
#include "FBXConverter.h"
#include "RasterizerStateManager.h"
#include "Renderer.h"
#include "SampleStateManager.h"
#include "ShaderManager.h"
#include "ShaderResourceManager.h"

// ==== TEMP
#include "RenderContext.h"
#include "RenderMaterial.h"
#include "RenderShape.h"

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

	// === Initialize the Renderer
	Renderer::GetInstance()->Initialize(m_Window, 1, DEFAULT_HEIGHT, DEFAULT_WIDTH);

	// === Initialize the Managers
	BlendStateManager::GetInstance();
	ConstantBufferManager::GetInstance();
	RasterizerStateManager::GetInstance();
	SampleStateManager::GetInstance();
	ShaderManager::GetInstance();
	ShaderResourceManager::GetInstance();

	// === TEMPORARY
	SetupScene();
}

Application::~Application()
{
	// === Shut everything down
	ShaderResourceManager::GetInstance()->Terminate();
	ShaderManager::GetInstance()->Terminate();
	SampleStateManager::GetInstance()->Terminate();
	RasterizerStateManager::GetInstance()->Terminate();
	ConstantBufferManager::GetInstance()->Terminate();
	BlendStateManager::GetInstance()->Terminate();

	Renderer::GetInstance()->Terminate();
}
// ==================================== //

// ===== Interface ===== //
bool Application::Run() 
{
	// === Update the Scene 
	ToShaderScene toShaderScene;
	toShaderScene.SceneViewMatrix = Renderer::GetInstance()->GetViewMatrix();
	toShaderScene.SceneProjectionMatrix = Renderer::GetInstance()->GetProjectionMatrix();

	ConstantBufferManager::GetInstance()->ApplySceneBuffer(&toShaderScene);
	Renderer::GetInstance()->Render();
	return true;
}
// ===================== //

// ===== Private Interface ===== //
void Application::SetupScene()
{
	Object* object = Object::Create();
	FBXConverter* fbxConverter = FBXConverter::GetInstance();
	fbxConverter->LoadFBX("Cube.fbx", object);

	RenderContext* context = new RenderContext();
	RenderMaterial* material = new RenderMaterial();
	RenderShape* shape = new RenderShape();
	shape->SetObject(object);

	Renderer::GetInstance()->AddForRendering(context, material, shape);
}
// ============================= //
