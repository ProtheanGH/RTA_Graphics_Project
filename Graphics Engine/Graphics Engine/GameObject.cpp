#include "GameObject.h"

#include "FBXConverter.h"
#include "ObjectManager.h"
#include "Renderer.h"

// ===== Constructor / Destructor ===== //
GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}
// ==================================== //

// ===== Interface ===== //
void GameObject::LoadGameObject()
{
	RenderNodeDirectory* nodeDirectory = RenderNodeDirectory::GetInstance();

	// === Create the Object
	Object* object = ObjectManager::GetInstance()->CreateNewObject();
	if (!FBXConverter::GetInstance()->LoadFBX(m_MeshPath.c_str(), object))
		return;
	object->SetPositionVector(m_WorldPosition);
	object->RotateX(m_WorldRotation.x);
	object->RotateY(m_WorldRotation.y);
	object->RotateZ(m_WorldRotation.z);
	object->Scale(m_WorldScale.x, m_WorldScale.y, m_WorldScale.z);

	// === Load the Components
	LoadComponents(object);

	// === Create the RenderContext
	RenderContext* context = nodeDirectory->CreateRenderContext(m_VertexShader, m_PixelShader, m_BlendState, m_RasterizerState, m_InputLayout);

	// === Create the RenderMaterial
	RenderMaterial* material = nodeDirectory->CreateRenderMaterial(m_SampleState, m_TextureIDs);

	// === Create the RenderShape
	RenderShape* shape = nodeDirectory->CreateRenderShape();
	shape->SetObject(object);
	shape->SetTopology(m_TopologyType);
	shape->SetFunc(m_RenderFunc);

	// === Add to the Renderer
	AddToRenderer(context, material, shape);
}
// ===================== //

// ===== Private Interface ===== //
void GameObject::AddToRenderer(RenderContext* _context, RenderMaterial* _material, RenderShape* _shape)
{
	Renderer* renderer = Renderer::GetInstance();

	// === Add the Root Object
	renderer->AddForRendering(_context, _material, _shape);

	// === Add all the Children
	RenderShape* childShape;
	size_t childCount = _shape->GetShapeObject()->GetChildren().size();
	for (size_t i = 0; i < childCount; ++i) {
		childShape = RenderNodeDirectory::GetInstance()->CreateRenderShape();
		childShape->SetObject(_shape->GetShapeObject()->GetChildren()[i]);
		childShape->SetTopology(m_TopologyType);
		childShape->SetFunc(_shape->GetFunc());

		AddToRenderer(_context, _material, childShape);
	}
}

void GameObject::LoadComponents(Object* _object)
{
	// === Needs to be overloaded if the GameObject has any components. 
	//		- They should ALL be loaded here
}
// ============================= //
