#include "Application.h"

#include "Renderer.h"
#include "BlendStateManager.h"
#include "ConstantBufferManager.h"
#include "FBXConverter.h"
#include "Writer.h"
#include "ObjectManager.h"
#include "RasterizerStateManager.h"
#include "Renderer.h"
#include "RenderNodeDirectory.h"
#include "SampleStateManager.h"
#include "ShaderManager.h"
#include "ShaderResourceManager.h"
#include "VertexFormats.h"

#include <ctime>

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
	Writer::Terminate();

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
	toShaderScene.SceneViewMatrix = m_Camera.GetViewMatrix();
	toShaderScene.SceneProjectionMatrix = Renderer::GetInstance()->GetProjectionMatrix();
	ConstantBufferManager::GetInstance()->ApplySceneBuffer(&toShaderScene);
	
	UpdateLighting();

	ObjectManager::GetInstance()->UpdateObjects();
	
	Renderer::GetInstance()->Render();
	return true;
}
/*static*/ Camera Application::GetCamera()
{
	return m_Camera;
}
// ===================== //

// ===== Private Interface ===== //
void Application::SetupScene()
{
	// === Teddy
	Object* object = ObjectManager::GetInstance()->CreateNewObject();
	FBXConverter* fbxConverter = FBXConverter::GetInstance();
	fbxConverter->LoadFBX("Teddy_Idle", object);
	
	RenderContext* context = RenderNodeDirectory::GetInstance()->CreateRenderContext(VertexShaderEnum::NormalMap_Vertex, PixelShaderEnum::NormalMap_Pixel, BlendStates::BlendState_Default, RasterizerStates::RasterizerState_Default, InputLayouts::NormalMapped_InputLayout);

	RenderMaterial* material = RenderNodeDirectory::GetInstance()->CreateRenderMaterial();
	material->AddShaderResourceID(ShaderResourceManager::GetInstance()->LoadTextureFromFile("Assets/Teddy_D.dds"));
	material->AddShaderResourceID(ShaderResourceManager::GetInstance()->LoadTextureFromFile("Assets/Teddy_Normal.dds"));

	RenderShape* shape = RenderNodeDirectory::GetInstance()->CreateRenderShape();
	shape->SetObject(object);

	Renderer::GetInstance()->AddForRendering(context, material, shape);
	CreateRenderShapes(context, material, object);
	// ===

	// === Bones
#if 0
	Object* bones = ObjectManager::GetInstance()->CreateNewObject();
	fbxConverter->LoadFBX("Bone", bones);
	Mesh* mesh = new Mesh();
	mesh->copy(bones->GetChildren()[0]->GetMesh());
	Bone* newBone = new Bone;
	fbxConverter->LoadSkeleton("Teddy_Idle", newBone);

	Object::CreateObjectFromSkeleton(newBone, *bones, mesh);
	bones->GetTransform().SetLocalMatrix(XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 200, 0, 0, 1));

	context = RenderNodeDirectory::GetInstance()->CreateRenderContext();

	material = RenderNodeDirectory::GetInstance()->CreateRenderMaterial();

	shape = RenderNodeDirectory::GetInstance()->CreateRenderShape();
	shape->SetObject(bones);

	Renderer::GetInstance()->AddForRendering(context, material, shape);

	CreateRenderShapes(context, material, bones);
#endif
	// ===

	// === Skybox === //
#pragma region Not_Loading_Correctly
#if 1
	Object* skybox = ObjectManager::GetInstance()->CreateNewObject();
	skybox->AddComponent(new SkyboxComponent(skybox));
	fbxConverter->LoadFBX("Cube", skybox);

	context = RenderNodeDirectory::GetInstance()->CreateRenderContext(
		VertexShaderEnum::Skybox_Vertex, PixelShaderEnum::Skybox_Pixel, BlendStates::BlendState_Default,
		RasterizerStates::Front_Culling, InputLayouts::SkyboxMapped_InputLayout);

	material = RenderNodeDirectory::GetInstance()->CreateRenderMaterial();
	material->AddShaderResourceID(ShaderResourceManager::GetInstance()->LoadTextureFromFile("Assets/Skybox_Texture.dds"));

	shape = RenderNodeDirectory::GetInstance()->CreateRenderShape();
	shape->SetObject(skybox);

	Renderer::GetInstance()->AddForRendering(context, material, shape);

	for (unsigned int i = 0; i < skybox->GetChildren().size(); ++i) {
		shape = RenderNodeDirectory::GetInstance()->CreateRenderShape();
		shape->SetObject(skybox->GetChildren()[i]);
		Renderer::GetInstance()->AddForRendering(context, material, shape);
	}
#endif
#pragma endregion
	// ============== //
}

void Application::UpdateLighting()
{

	ToShaderLight toShaderLight;

	toShaderLight.ambientLight.color = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	toShaderLight.directionalLight.color     = XMFLOAT4(0.78823f, 0.88627f, 1.0f, 1.0f);	// Color of overcast sky
	toShaderLight.directionalLight.direction = XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f);

	toShaderLight.pointLight.color    = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);	// Blue
	toShaderLight.pointLight.position = XMFLOAT4(0.0f, 100.0f, 40.0f, 0.0f);
	toShaderLight.pointLight.radius   = 20.0f;

	toShaderLight.spotLight.position  = XMFLOAT4(0.0f, 200.0f, 100.0f, 0.0f);
	toShaderLight.spotLight.direction = XMFLOAT4(0.0f, -1.0f, 1.0f, 0.0f);
	toShaderLight.spotLight.color     = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);	// Green
	toShaderLight.spotLight.coneRatio = XMFLOAT2(0.1f, 0.2f);

	ConstantBufferManager::GetInstance()->ApplyLightBuffer(&toShaderLight);
}

void Application::CreateRenderShapes(RenderContext* _context, RenderMaterial* _material, Object* _object)
{
	RenderShape* shape;
	for (unsigned int i = 0; i < _object->GetChildren().size(); ++i) {
		shape = RenderNodeDirectory::GetInstance()->CreateRenderShape();
		shape->SetObject(_object->GetChildren()[i]);

		Renderer::GetInstance()->AddForRendering(_context, _material, shape);

		CreateRenderShapes(_context, _material, _object->GetChildren()[i]);
	}
}
// ============================= //
