#pragma once
#include <DirectXMath.h>
#include <vector>

class Bone{

private:
	std::string name;
	DirectX::XMFLOAT4X4 localMatrix;
	DirectX::XMFLOAT4X4 worldMatrix;
	Bone* parent;
	std::vector<Bone*> children;
	int index;

	DirectX::XMFLOAT4X4 globalInverseBindPose;

public:
	Bone() = default;
	~Bone() = default;

	const DirectX::XMFLOAT4X4& GetWorld();
	inline DirectX::XMFLOAT4X4& GetLocal(){ return localMatrix; }
	inline DirectX::XMFLOAT4X4& GetGlobalBindPose(){ return globalInverseBindPose; }

	inline std::string& GetName(){ return name; }
	inline std::vector<Bone*>& GetChildren(){ return children; }

	inline Bone* GetParent(){ return parent; }
	inline void SetParent(Bone* bone){ parent = bone; }

	inline void SetIndex(int _index){ index = _index; }
	inline int GetIndex()const { return index; }

	void Destroy();

	static Bone* FindBone(Bone* _Bone, std::string& _BoneName);
	static void CreateBoneList(Bone* _bone, std::vector<Bone*>& boneList);
};
