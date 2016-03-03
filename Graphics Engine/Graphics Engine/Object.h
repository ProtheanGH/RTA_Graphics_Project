#pragma once

#include "Transform.h"
#include "Mesh.h"
#include <string>

class Object{

private:
	Transform transform;
	Mesh* mesh;
	Object* parent;
	std::vector<Object*> children;
	std::string name;

	Object();
	~Object();

public:

	inline Transform& GetTransform()  { return transform; }
	inline std::vector<Object*>& GetChildren() { return children; }

	inline Mesh* GetMesh() { return mesh; }
	inline void SetMesh(Mesh* _mesh){ mesh = _mesh; }

	inline Object* GetParent(){ return parent; }
	inline void SetParent(Object* _parent){ parent = _parent; }

	inline std::string& GetName() { return name; }
	inline void SetName(std::string _name){ name = _name; }

	static Object* Create();

	void Destroy();
};