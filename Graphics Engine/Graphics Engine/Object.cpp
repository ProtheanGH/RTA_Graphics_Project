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
	Destroy();
}

void Object::SetMesh(Mesh* _mesh){

	if (_mesh != nullptr){
		mesh = _mesh;
		SetBuffers();
	}
}

void Object::SetBuffers(){

	ReleaseBuffers();

	Renderer* renderer = Renderer::GetInstance();

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex_POSNORMUV)* mesh->GetVerts().size();
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
	indexBufferDesc.ByteWidth = sizeof(unsigned int)* mesh->GetIndices().size();
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

	for (unsigned int i = 0; i < children.size(); ++i){
		children[i]->Destroy();
	}

	delete mesh;

	delete this;
}