#include "Interpolater.h"

Interpolater* Interpolater::instance = nullptr;

Interpolater* Interpolater::GetInstance(){
	if (instance == nullptr){
		instance = new Interpolater();
	}

	return instance;
}

void Interpolater::Terminate(){
	delete instance;
}

void Interpolater::AddTime(float time){
	SetTime(curTime + time);
}

void Interpolater::SetTime(float time){
	if (animation == nullptr)
		curTime = 0.0f;
	else{
		curTime = time;
		if (curTime > animation->GetDuration())
			curTime -= animation->GetDuration();
		else if (curTime < 0.0f)
			curTime += animation->GetDuration();
	}
}

float Interpolater::GetTime() const{
	return curTime;
}

void Interpolater::SetAnimation(Animation* _animation){
	animation = _animation;
}

void Interpolater::Update(float deltaTime){

	if (animation == nullptr) return;

	AddTime(deltaTime);

	unsigned int prevKey = 0;
	unsigned int nextKey = 1;

	std::vector<Animation::KeyFrame*>& keyFrames = animation->GetKeyFrames();
	while (true){
		if (keyFrames[prevKey]->time < curTime && keyFrames[nextKey]->time > curTime)
			break;

		if (keyFrames[prevKey]->time > curTime){
			prevKey = (unsigned int)keyFrames.size() - 1;
			nextKey = 0;
			break;
		}

		++prevKey;
		++nextKey;
	}

	float totalTime = 0.0f;
	float ratio = 0.0f;

	if (prevKey < nextKey){
		totalTime = keyFrames[nextKey]->time - keyFrames[prevKey]->time;
		ratio = (curTime - keyFrames[prevKey]->time) / totalTime;
	}
	else{
		ratio = (curTime / keyFrames[nextKey]->time);
	}

	unsigned int bone_count = (unsigned int)animation->GetKeyFrames().size();
	bones.clear(); 
	bones.resize(bone_count);

	for (unsigned int i = 0; i < bone_count; ++i){
		DirectX::XMFLOAT4 translation;
		translation.x = (keyFrames[nextKey]->keys[i]->translation.x - keyFrames[prevKey]->keys[i]->translation.x) * ratio + keyFrames[prevKey]->keys[i]->translation.x;
		translation.y = (keyFrames[nextKey]->keys[i]->translation.y - keyFrames[prevKey]->keys[i]->translation.y) * ratio + keyFrames[prevKey]->keys[i]->translation.y;
		translation.z = (keyFrames[nextKey]->keys[i]->translation.z - keyFrames[prevKey]->keys[i]->translation.z) * ratio + keyFrames[prevKey]->keys[i]->translation.z;

		DirectX::XMFLOAT4 rotation;
		rotation.x = (keyFrames[nextKey]->keys[i]->rotation.x - keyFrames[prevKey]->keys[i]->rotation.x) * ratio + keyFrames[prevKey]->keys[i]->rotation.x;
		rotation.y = (keyFrames[nextKey]->keys[i]->rotation.y - keyFrames[prevKey]->keys[i]->rotation.y) * ratio + keyFrames[prevKey]->keys[i]->rotation.y;
		rotation.z = (keyFrames[nextKey]->keys[i]->rotation.z - keyFrames[prevKey]->keys[i]->rotation.z) * ratio + keyFrames[prevKey]->keys[i]->rotation.z;

		DirectX::XMFLOAT4 scale;
		scale.x = (keyFrames[nextKey]->keys[i]->scale.x - keyFrames[prevKey]->keys[i]->scale.x) * ratio + keyFrames[prevKey]->keys[i]->scale.x;
		scale.y = (keyFrames[nextKey]->keys[i]->scale.y - keyFrames[prevKey]->keys[i]->scale.y) * ratio + keyFrames[prevKey]->keys[i]->scale.x;
		scale.z = (keyFrames[nextKey]->keys[i]->scale.z - keyFrames[prevKey]->keys[i]->scale.z) * ratio + keyFrames[prevKey]->keys[i]->scale.x;

		
		DirectX::XMMATRIX matrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		matrix = matrix * DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		matrix = matrix * DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);

		DirectX::XMMATRIX local_mat = DirectX::XMLoadFloat4x4(&bones[i].GetLocal());
		local_mat = local_mat * matrix;
		DirectX::XMStoreFloat4x4(&bones[i].GetLocal(), local_mat);
	}

}

const std::vector<Bone>& Interpolater::GetBones() const{
	return bones;
}
