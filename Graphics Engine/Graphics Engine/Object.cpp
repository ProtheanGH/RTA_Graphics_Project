#include "Object.h"
#include "Renderer.h"

Object* Object::Create(){
	return new Object;
}

Object::Object(){
	mesh = nullptr;
	parent = nullptr;
}

Object::~Object(){
//	Destroy();
}

void Object::SetMesh(Mesh* _mesh){

	if (_mesh != nullptr){
		mesh = _mesh;
		SetBuffers();
	}
}

void Object::SetRootBone(Bone* _bone){

	rootBone = _bone;
	for (unsigned int i = 0; i < children.size(); ++i){
		children[i]->SetRootBone(_bone);
	}
}

void Object::SetBuffers(){

	ReleaseBuffers();

	Renderer* renderer = Renderer::GetInstance();

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex_POSNORMUV)* (unsigned int)mesh->GetVerts().size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = mesh->GetVerts().data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	renderer->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int)* (unsigned int)mesh->GetIndices().size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = mesh->GetIndices().data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	renderer->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
}

void Object::ReleaseBuffers(){
	if (vertexBuffer)
		vertexBuffer->Release();
	if (indexBuffer)
		indexBuffer->Release();
}

void Object::Destroy(){

	ReleaseBuffers();

	if (rootBone != nullptr){
		rootBone->Destroy();
		delete rootBone;
		SetRootBone(nullptr);
	}

	for (unsigned int i = 0; i < children.size(); ++i){
		children[i]->Destroy();
		delete children[i];
	}

	for (unsigned int i = 0; i < components.size(); ++i)
	{
		components[i]->Destroy();
		delete components[i];
	}

	delete mesh;
}

void Object::Update()
{
	for (unsigned int i = 0; i < children.size(); ++i)
	{
		children[i]->Update();
	}

	for (unsigned int i = 0; i < components.size(); ++i)
	{
		components[i]->Update();
	}
}

DirectX::XMFLOAT4X4& Object::GetWorld(){
	if (parent != nullptr){
		DirectX::XMMATRIX local_mat = DirectX::XMLoadFloat4x4(&transform.GetLocalMatrix());
		DirectX::XMMATRIX world_mat = DirectX::XMLoadFloat4x4(&parent->GetWorld());

		local_mat = local_mat * world_mat;

		DirectX::XMStoreFloat4x4(&worldMatrix, local_mat);
	}
	else{

		DirectX::XMMATRIX local_mat = DirectX::XMLoadFloat4x4(&transform.GetLocalMatrix());
		DirectX::XMStoreFloat4x4(&worldMatrix, local_mat);
	}

	return worldMatrix;
}

void Object::CreateObjectFromSkeleton(Bone* _rootBone, Object& _object, Mesh* _mesh){
	unsigned int child_count = _rootBone->GetChildren().size();
	for (unsigned int i = 0; i < child_count; ++i){
		Object* child_object = Object::Create();
		Mesh* child_mesh = new Mesh();
		child_mesh->copy(_mesh);
		Bone* child_bone = _rootBone->GetChildren()[i];

		CreateObjectFromSkeleton(child_bone, *child_object, child_mesh);

		child_object->SetParent(&_object);
		_object.GetChildren().push_back(child_object);
	}

	DirectX::XMFLOAT4X4 world_matrix = _rootBone->GetLocal();
	_object.GetTransform().SetLocalMatrix(world_matrix);

	_object.SetMesh(_mesh);
}

void Object::AddComponent(Component* _comp)
{
	components.push_back(_comp);
}

DirectX::XMFLOAT4X4 Object::CreateZeroMatrix() const
{
	return DirectX::XMFLOAT4X4(0, 0, 0, 0,
							   0, 0, 0, 0,
							   0, 0, 0, 0,
							   0, 0, 0, 0);
}

void Object::SetWorld(const DirectX::XMFLOAT4X4& _mat)
{
	worldMatrix = _mat;
}

void Object::RotateX(const float& _degree)
{
	float r = XMConvertToRadians(_degree);

	DirectX::XMMATRIX localMat = DirectX::XMLoadFloat4x4(&worldMatrix);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationX(r);

	// Blame Jorge if this doesn't work
	localMat = rotation * localMat;

	DirectX::XMStoreFloat4x4(&worldMatrix, localMat);
}

void Object::RotateY(const float& _degree)
{
	float r = XMConvertToRadians(_degree);

	DirectX::XMMATRIX localMat = DirectX::XMLoadFloat4x4(&worldMatrix);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationY(r);

	// Blame Jorge if this doesn't work
	localMat = rotation * localMat;

	DirectX::XMStoreFloat4x4(&worldMatrix, localMat);
}

void Object::RotateZ(const float& _degree)
{
	float r = XMConvertToRadians(_degree);

	DirectX::XMMATRIX localMat = DirectX::XMLoadFloat4x4(&worldMatrix);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationZ(r);

	// Blame Jorge if this doesn't work
	localMat = rotation * localMat;

	DirectX::XMStoreFloat4x4(&worldMatrix, localMat);
}

void Object::Translate(const float& _x, const float& _y, const float& _z)
{
	DirectX::XMMATRIX localMat    = DirectX::XMLoadFloat4x4(&worldMatrix);
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(_x, _y, _z);

	// Blame Jorge if this doesn't work
	localMat = translation * localMat;

	DirectX::XMStoreFloat4x4(k&worldMatrix, localMat);
}

void Object::Scale(const float& _x, const float& _y, const float& _z)
{
	DirectX::XMMATRIX localMat = DirectX::XMLoadFloat4x4(&worldMatrix);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(_x, _y, _z);

	// Blame Jorge if this doesn't work
	localMat = scale * localMat;

	DirectX::XMStoreFloat4x4(&worldMatrix, localMat);
}

