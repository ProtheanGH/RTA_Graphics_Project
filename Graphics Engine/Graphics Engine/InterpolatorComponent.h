#pragma once
#include "Component.h"

#include "Animation.h"
#include "Bone.h"

class InterpolatorComponent :
	public Component
{
private:
	float curTime;
	Animation* animation;
	std::vector<Bone> bones;

public:
	// === Constructor / Destructor === //
	InterpolatorComponent(Object* _parent = nullptr);
	virtual ~InterpolatorComponent();
	// ================================ //

	// === Interface === //
	virtual bool Update(float _deltaTime);
	virtual void Destroy(void);

	void AddTime(float time);
	void SetTime(float time);
	void SetAnimation(Animation* animation);
	// ================= //

	// === Accessors === //
	inline const std::vector<Bone>& GetBones() const {
		return bones;
	}
	inline float GetTime() const {
		return curTime;
	}
	// ================= //
};

