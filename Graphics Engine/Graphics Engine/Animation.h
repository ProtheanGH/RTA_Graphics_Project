#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "Bone.h"

class Animation{

public:

	struct Key{
		float time;
		DirectX::XMFLOAT4 translation, rotation, scale;
	};

	struct KeyFrame{
		std::string name;
		std::vector<Key > keys;

		bool FindKey(float time, Key& key){
			for (unsigned int i = 0; i < keys.size(); ++i){
				if (keys[i].time == time){
					key = keys[i];
					return true;
				}	
			}

			return false;
		}
	};

public:
	Animation() = default;
	~Animation(){
		for (unsigned int i = 0; i < keyFrames.size(); ++i){
			delete keyFrames[i];
		}

		keyFrames.clear();
	}

	inline float GetDuration() const { return duration; }
	inline std::string& GetName(){ return name; }
	inline std::vector<KeyFrame*>& GetKeyFrames(){ return keyFrames; }
	inline std::vector<Bone*>& GetBones(){ return bones; }

	KeyFrame* FindKeyFrame(std::string frameName){
		for (unsigned int i = 0; i < keyFrames.size(); ++i){
			if (keyFrames[i]->name == frameName){
				return keyFrames[i];
			}
		}

		return nullptr;
	}

	void CalculateDuration(){
		if (keyFrames.size() == 0)
			duration = 0.0f;
		else
			duration = keyFrames[0]->keys.back().time;
	}

private:
	std::string name;
	float duration;

	std::vector< KeyFrame* > keyFrames;

	std::vector<Bone*> bones;
};