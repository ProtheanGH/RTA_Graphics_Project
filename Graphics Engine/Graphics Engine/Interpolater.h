#pragma once

#include "Animation.h"
#include "Bone.h"

class Interpolater{

private:
	Interpolater() = default;
	~Interpolater() = default;

	static Interpolater* instance;

	float curTime;
	Animation* animation;
	std::vector<Bone> bones;

public:

	void AddTime(float time);
	void SetTime(float time);
	float GetTime() const;

	void SetAnimation(Animation* animation);

	const std::vector<Bone>& GetBones() const;

	void Update(float deltaTime);

	static Interpolater* GetInstance();
	static void Terminate();
};