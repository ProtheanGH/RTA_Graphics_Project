#pragma once
#include <DirectXMath.h>
#include <vector>

struct BoneInfluence{
	BoneInfluence(unsigned i, float w){
		index = i;
		weight = w;
	}
	unsigned int index;
	float weight;
};

class Bone{

private:
	std::string name;
	DirectX::XMFLOAT4X4 localMatrix;
	DirectX::XMFLOAT4X4 worldMatrix;
	Bone* parent;
	std::vector<Bone*> children;
	std::vector<BoneInfluence> boneInfluence;

public:
	Bone() = default;
	~Bone() = default;

	const DirectX::XMFLOAT4X4& GetWorld();
	inline DirectX::XMFLOAT4X4& GetLocal(){ return localMatrix; }

	inline std::string& GetName(){ return name; }
	inline std::vector<Bone*>& GetChildren(){ return children; }

	inline std::vector<BoneInfluence>& GetBoneInfluence(){ return boneInfluence; }

	inline Bone* GetParent(){ return parent; }
	inline void SetParent(Bone* bone){ parent = bone; }

	void Destroy();

	static Bone* FindBone(Bone* _Bone, std::string& _BoneName);
	static void CreateBoneList(Bone* _bone, std::vector<Bone*>& boneList);
};
