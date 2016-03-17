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
	Animation::KeyFrame* curKeyFrame = animation->GetKeyFrames()[0];

	while (true){
		if (curKeyFrame->keys[prevKey].time < curTime && curKeyFrame->keys[nextKey].time > curTime)
			break;

		if (curKeyFrame->keys[prevKey].time > curTime){
			prevKey = curKeyFrame->keys.size() - 1;
			nextKey = 0;
			break;
		}

		++prevKey;
		++nextKey;
	}

	float totalTime = 0.0f;
	float ratio = 0.0f;

	if (prevKey < nextKey){
		totalTime = curKeyFrame->keys[nextKey].time - curKeyFrame->keys[prevKey].time;
		ratio = (curTime - curKeyFrame->keys[prevKey].time) / totalTime;
	}
	else{
		ratio = (curTime / curKeyFrame->keys[nextKey].time);
	}

	unsigned int bone_count = animation->GetBones().size();
	bones.clear(); 
	bones.resize(bone_count);

	for (unsigned int i = 0; i < bone_count; ++i){
		DirectX::XMFLOAT4 translation;
		translation.x = (animation->GetKeyFrames()[i]->keys[nextKey].translation.x - animation->GetKeyFrames()[i]->keys[prevKey].translation.x) * ratio;
		translation.y = (animation->GetKeyFrames()[i]->keys[nextKey].translation.y - animation->GetKeyFrames()[i]->keys[prevKey].translation.y) * ratio;
		translation.z = (animation->GetKeyFrames()[i]->keys[nextKey].translation.z - animation->GetKeyFrames()[i]->keys[prevKey].translation.z) * ratio;

		DirectX::XMFLOAT4 rotation;
		rotation.x = (animation->GetKeyFrames()[i]->keys[nextKey].rotation.x - animation->GetKeyFrames()[i]->keys[prevKey].rotation.x) * ratio;
		rotation.y = (animation->GetKeyFrames()[i]->keys[nextKey].rotation.y - animation->GetKeyFrames()[i]->keys[prevKey].rotation.y) * ratio;
		rotation.z = (animation->GetKeyFrames()[i]->keys[nextKey].rotation.z - animation->GetKeyFrames()[i]->keys[prevKey].rotation.z) * ratio;

		DirectX::XMFLOAT4 scale;
		scale.x = (animation->GetKeyFrames()[i]->keys[nextKey].scale.x - animation->GetKeyFrames()[i]->keys[prevKey].scale.x) * ratio;
		scale.y = (animation->GetKeyFrames()[i]->keys[nextKey].scale.y - animation->GetKeyFrames()[i]->keys[prevKey].scale.y) * ratio;
		scale.z = (animation->GetKeyFrames()[i]->keys[nextKey].scale.z - animation->GetKeyFrames()[i]->keys[prevKey].scale.z) * ratio;

		
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
