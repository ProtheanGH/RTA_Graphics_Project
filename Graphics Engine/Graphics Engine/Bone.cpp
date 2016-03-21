#include "Bone.h"

const DirectX::XMFLOAT4X4& Bone::GetWorld(){

	if (parent != nullptr){
		DirectX::XMMATRIX local_mat = DirectX::XMLoadFloat4x4(&localMatrix);
		DirectX::XMMATRIX world_mat = DirectX::XMLoadFloat4x4(&parent->GetWorld());

		local_mat = local_mat * world_mat;

		DirectX::XMStoreFloat4x4(&worldMatrix, local_mat);
	}
	else{
		DirectX::XMMATRIX local_mat = DirectX::XMLoadFloat4x4(&localMatrix);
		DirectX::XMStoreFloat4x4(&worldMatrix, local_mat);
	}

	return worldMatrix;
}

void Bone::Destroy(){
	for (unsigned int i = 0; i < children.size(); ++i){
		children[i]->Destroy();
		delete children[i];
	}
}

Bone* Bone::FindBone(Bone* _Bone, std::string& _BoneName){
	Bone* temp = nullptr;

	if (_Bone->GetName() == _BoneName) return _Bone;

	for (unsigned int i = 0; i < _Bone->GetChildren().size(); ++i){
		temp = FindBone(_Bone->GetChildren()[i], _BoneName);
		if (temp != nullptr) 
			return temp;
	}
	return nullptr;
}

void Bone::CreateBoneList(Bone* _bone, std::vector<Bone*>& boneList){
	
	_bone->SetIndex((unsigned int)boneList.size());
	boneList.push_back(_bone);

	for (unsigned int i = 0; i < _bone->GetChildren().size(); ++i){
		CreateBoneList(_bone->GetChildren()[i], boneList);
	}
}
