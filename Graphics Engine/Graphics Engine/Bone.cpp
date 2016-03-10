#include "Bone.h"

const DirectX::XMFLOAT4X4& Bone::GetWorld(){

	if (parent != nullptr){
		//set world matrix to local matrix
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