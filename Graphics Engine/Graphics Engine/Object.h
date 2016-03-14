#pragma once

#include "Transform.h"
#include "Mesh.h"
#include <string>
#include <d3d11.h>
#include "Bone.h"

class Object{

private:
	Transform transform;
	Mesh* mesh;
	Object* parent;
	std::vector<Object*> children;
	std::string name;
	Bone* rootBone;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Object();

	void SetBuffers();
	void ReleaseBuffers();
	

public:

	static Object* Create();
	~Object();

	void Destroy();

	inline Transform& GetTransform()  { return transform; }
	inline std::vector<Object*>& GetChildren() { return children; }

	inline Bone* GetRootBone(){ return rootBone; }
	void SetRootBone(Bone* bone);

	inline Mesh* GetMesh() { return mesh; }
	void SetMesh(Mesh* mesh);

	inline Object* GetParent(){ return parent; }
	inline void SetParent(Object* _parent){ parent = _parent; }

	inline std::string& GetName() { return name; }
	inline void SetName(std::string _name){ name = _name; }

	inline ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; }
	inline ID3D11Buffer* GetIndexBuffer(){ return indexBuffer; }
};