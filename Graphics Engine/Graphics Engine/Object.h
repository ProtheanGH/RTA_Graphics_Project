#pragma once

#include "Transform.h"
#include "Mesh.h"
#include <string>
#include <d3d11.h>
#include "Bone.h"
#include "Component.h"
#include <vector>
#include <cmath>
#include "InterpolatorComponent.h"

class Object{

private:
	DirectX::XMFLOAT4X4 worldMatrix;
	Transform transform;
	Mesh* mesh;
	Object* parent;
	std::vector<Object*> children;
	std::string name;
	Bone* rootBone;
	std::vector<Animation*> animations;

	std::vector<Component*> components;
	InterpolatorComponent* Interpolator;

	std::vector<std::string> textureNames;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	Object();

	void SetBuffers();
	void ReleaseBuffers();

	// Matrix Helpers
	DirectX::XMFLOAT4X4 CreateZeroMatrix ( void ) const;

public:

	static Object* Create();
	~Object();

	void Destroy();

	void Update(float _deltaTime);
	void AddComponent(Component* _comp);

	inline Transform& GetTransform()  { return transform; }
	inline std::vector<Object*>& GetChildren() { return children; }

	inline Bone* GetRootBone(){ return rootBone; }
	void SetRootBone(Bone* bone);

	inline Mesh* GetMesh() { return mesh; }
	void SetMesh(Mesh* mesh);

	inline Object* GetParent(){ return parent; }
	inline void SetParent(Object* _parent){ parent = _parent; }

	inline std::string& GetName() { return name; }
	inline void SetName(std::string& _name){ name = _name; }

	inline std::vector<std::string>& GetTextureNames(){ return textureNames; }

	inline ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; }
	inline ID3D11Buffer* GetIndexBuffer(){ return indexBuffer; }

	inline void AddAnimation(Animation* _anim) {
		animations.push_back(_anim);
	}
	inline std::vector<Animation*>& GetAnimations() {
		return animations;
	}

	inline void AddInterpolator() {
		Interpolator = new InterpolatorComponent(this);
	}
	inline InterpolatorComponent* GetInterpolator() {
		return Interpolator;
	}

	DirectX::XMFLOAT4X4& GetWorld();
	void SetWorld(const DirectX::XMFLOAT4X4& _mat);

	// Matrix Manipulations
	void RotateX  ( const float& _degree );
	void RotateY  ( const float& _degree );
	void RotateZ  ( const float& _degree );
	void Translate( const float& _x, const float& _y, const float& _z );
	void Scale    ( const float& _x, const float& _y, const float& _z );

	void SetPositionVector( DirectX::XMFLOAT3 _location );
};