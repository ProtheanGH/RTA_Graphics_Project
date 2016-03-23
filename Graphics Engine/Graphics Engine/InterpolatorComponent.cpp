#include "InterpolatorComponent.h"

#include "Object.h"

// ===== Constructor / Destructor ===== //
InterpolatorComponent::InterpolatorComponent(Object* _parent)
{
	Bone::CreateBoneList(_parent->GetRootBone(), bones);
}

InterpolatorComponent::~InterpolatorComponent()
{

}
// ==================================== //


// ===== Interface ===== //
bool InterpolatorComponent::Update(float _deltaTime)
{
	if (animation == nullptr)
		return false;

	AddTime(_deltaTime);

	unsigned int prevKey = 0;
	unsigned int nextKey = 1;

	std::vector<Animation::KeyFrame*>& keyFrames = animation->GetKeyFrames();
	while (true){
		if (keyFrames[prevKey]->time <= curTime && keyFrames[nextKey]->time > curTime)
			break;

		float prevTime = keyFrames[prevKey]->time;
		if (keyFrames[prevKey]->time >= curTime || curTime >= animation->GetDuration()){
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

	unsigned int bone_count = (unsigned int)animation->GetKeyFrames()[prevKey]->keys.size();
//	bones.clear();
//	bones.resize(bone_count);

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
		scale.y = (keyFrames[nextKey]->keys[i]->scale.y - keyFrames[prevKey]->keys[i]->scale.y) * ratio + keyFrames[prevKey]->keys[i]->scale.y;
		scale.z = (keyFrames[nextKey]->keys[i]->scale.z - keyFrames[prevKey]->keys[i]->scale.z) * ratio + keyFrames[prevKey]->keys[i]->scale.z;


		DirectX::XMMATRIX matrix = DirectX::XMMatrixScaling(keyFrames[prevKey]->keys[i]->scale.x, keyFrames[prevKey]->keys[i]->scale.y, keyFrames[prevKey]->keys[i]->scale.z);
		matrix = matrix * DirectX::XMMatrixRotationRollPitchYaw(keyFrames[prevKey]->keys[i]->rotation.x, keyFrames[prevKey]->keys[i]->rotation.y, keyFrames[prevKey]->keys[i]->rotation.z);
		matrix = matrix * DirectX::XMMatrixTranslation(keyFrames[prevKey]->keys[i]->translation.x, keyFrames[prevKey]->keys[i]->translation.y, keyFrames[prevKey]->keys[i]->translation.z);

//		DirectX::XMMATRIX matrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//		matrix = matrix * DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
//		matrix = matrix * DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);

//		DirectX::XMMATRIX local_mat = DirectX::XMLoadFloat4x4(&bones[i]->GetLocal());
//		local_mat = local_mat * matrix;
		DirectX::XMStoreFloat4x4(&bones[i]->GetLocal(), matrix);
	}
	return true;
}

void InterpolatorComponent::Destroy(void)
{

}

void InterpolatorComponent::AddTime(float time)
{
	SetTime(curTime + time);
}

void InterpolatorComponent::SetTime(float time)
{
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

void InterpolatorComponent::SetAnimation(Animation* _animation)
{
	animation = _animation;
}
// ===================== //