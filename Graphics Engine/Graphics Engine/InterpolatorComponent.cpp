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

	AddTime(_deltaTime / 100);

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

	for (unsigned int i = 0; i < bone_count; ++i){

		DirectX::XMFLOAT4 translation = interpolate(keyFrames[prevKey]->keys[i]->translation, keyFrames[nextKey]->keys[i]->translation, ratio);

		DirectX::XMFLOAT4 rotation = interpolate(keyFrames[prevKey]->keys[i]->rotation, keyFrames[nextKey]->keys[i]->rotation, ratio);

		DirectX::XMFLOAT4 scale = interpolate(keyFrames[prevKey]->keys[i]->scale, keyFrames[nextKey]->keys[i]->scale, ratio);

		/*int keyFrameIndex = 0;
		DirectX::XMFLOAT4 translation = keyFrames[keyFrameIndex]->keys[i]->translation;
		DirectX::XMFLOAT4 rotation = keyFrames[keyFrameIndex]->keys[i]->rotation;
		DirectX::XMFLOAT4 scale = keyFrames[keyFrameIndex]->keys[i]->scale;*/
		
		DirectX::XMMATRIX matrix = DirectX::XMMatrixScaling(scale.x,scale.y,scale.z);
		matrix = matrix * DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		matrix = matrix * DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);

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
		if (curTime >= animation->GetDuration())
			curTime -= animation->GetDuration();
		else if (curTime < 0.0f)
			curTime += animation->GetDuration();
	}
}

void InterpolatorComponent::SetAnimation(Animation* _animation)
{
	animation = _animation;
}

DirectX::XMFLOAT4 InterpolatorComponent::interpolate(DirectX::XMFLOAT4& start, DirectX::XMFLOAT4& end, float ratio){

	DirectX::XMFLOAT4 result;

	result.x = start.x * (1 - ratio) + end.x * ratio;
	result.y = start.y * (1 - ratio) + end.y * ratio;
	result.z = start.z * (1 - ratio) + end.z * ratio;
	result.w = start.w * (1 - ratio) + end.w * ratio;

	return result;
}

// ===================== //