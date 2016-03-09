#include "Application.h"

#include "Renderer.h"
#include "BlendStateManager.h"
#include "ConstantBufferManager.h"
#include "FBXConverter.h"
#include "ObjectManager.h"
#include "RasterizerStateManager.h"
#include "Renderer.h"
#include "RenderNodeDirectory.h"
#include "SampleStateManager.h"
#include "ShaderManager.h"
#include "ShaderResourceManager.h"
#include "VertexFormats.h"

#include <ctime>

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
	InputLayoutManager::GetInstance();
	ObjectManager::GetInstance();
	RasterizerStateManager::GetInstance();
	RenderNodeDirectory::GetInstance();
	SampleStateManager::GetInstance();
	ShaderManager::GetInstance();
	ShaderResourceManager::GetInstance();

	m_Time = (unsigned long)GetTickCount64();

	// === TEMPORARY
	SetupScene();
}

Application::~Application()
{
	// === Shut everything down
	ShaderResourceManager::GetInstance()->Terminate();
	ShaderManager::GetInstance()->Terminate();
	SampleStateManager::GetInstance()->Terminate();
	RenderNodeDirectory::GetInstance()->Terminate();
	RasterizerStateManager::GetInstance()->Terminate();
	ObjectManager::GetInstance()->Terminate();
	InputLayoutManager::GetInstance()->Terminate();
	ConstantBufferManager::GetInstance()->Terminate();
	BlendStateManager::GetInstance()->Terminate();
	FBXConverter::GetInstance()->Terminate();

	Renderer::GetInstance()->Terminate();
}
// ==================================== //

// ===== Interface ===== //
bool Application::Run() 
{
	// === Update the Time (in seconds)
	float deltaTime = (float)((GetTickCount64() - (unsigned long long)m_Time));
	deltaTime /= 1000;
	m_Time = GetTickCount();

	// === Update the Camera
	m_Camera.Update(deltaTime);

	// === Update the Scene 
	ToShaderScene toShaderScene;
	ToShaderLight toShaderLight;
	toShaderScene.SceneViewMatrix = m_Camera.GetViewMatrix();
	toShaderScene.SceneProjectionMatrix = Renderer::GetInstance()->GetProjectionMatrix();
	toShaderLight.diffuseColor     = XMFLOAT4( 201.0f, 226.0f, 255.0f, 1.0f );	// Color of overcast sky
	toShaderLight.diffuseDirection = XMFLOAT4( 1.0f,  -1.0f,   1.0f,   1.0f );
	toShaderLight.pointColor       = XMFLOAT4( 0.0f,   0.0f,   0.0f,   0.0f );
	toShaderLight.pointPosition    = XMFLOAT4( 0.0f,   0.0f,   0.0f,   0.0f );
	toShaderLight.spotPosition     = XMFLOAT4( 0.0f,   0.0f,   0.0f,   0.0f );
	toShaderLight.spotDirection    = XMFLOAT4( 0.0f,   0.0f,   0.0f,   0.0f );
	toShaderLight.spotColor        = XMFLOAT4( 0.0f,   0.0f,   0.0f,   0.0f );
	toShaderLight.spotConeRatio    = XMFLOAT4( 0.0f,   0.0f,   0.0f,   0.0f );
	ConstantBufferManager::GetInstance()->ApplySceneBuffer(&toShaderScene);
	ConstantBufferManager::GetInstance()->ApplyLightBuffer(&toShaderLight);
	
	Renderer::GetInstance()->Render();
	return true;
}
// ===================== //

// ===== Private Interface ===== //
void Application::SetupScene()
{
	Object* object = ObjectManager::GetInstance()->CreateNewObject();
	FBXConverter* fbxConverter = FBXConverter::GetInstance();
	fbxConverter->LoadFBX("Cube.fbx", object);

	RenderContext* context = RenderNodeDirectory::GetInstance()->CreateRenderContext();
	RenderMaterial* material = RenderNodeDirectory::GetInstance()->CreateRenderMaterial();
	RenderShape* shape = RenderNodeDirectory::GetInstance()->CreateRenderShape();
	shape->SetObject(object);

	Renderer::GetInstance()->AddForRendering(context, material, shape);

	for (unsigned int i = 0; i < object->GetChildren().size(); ++i) {
		shape = RenderNodeDirectory::GetInstance()->CreateRenderShape();
		shape->SetObject(object->GetChildren()[i]);

		Renderer::GetInstance()->AddForRendering(context, material, shape);
	}
}
// ============================= //
