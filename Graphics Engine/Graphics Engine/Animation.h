#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "Bone.h"

class Animation{

public:

	struct Key{
		std::string name;
		DirectX::XMFLOAT4 translation, rotation, scale;
	};

	struct KeyFrame{
		float time;
		std::vector<Key*> keys;

		Key* FindKey(std::string name){
			for (unsigned int i = 0; i < keys.size(); ++i){
				if (keys[i]->name == name)
					return keys[i];
			}

			return nullptr;
		}

		~KeyFrame() {
			Destroy();
		}

	private :
		void Destroy() {
			unsigned int size = (unsigned int)keys.size();
			for (unsigned int i = 0; i < size; ++i) {
				delete keys[i];
			}

			keys.clear();
		}
	};

public:
	Animation() = default;
	~Animation(){
		Destroy();
	}

	inline float GetDuration() const { return duration; }
	inline std::string& GetName(){ return name; }
	inline std::vector<KeyFrame*>& GetKeyFrames(){ return keyFrames; }
	inline std::vector<Bone*>& GetBones(){ return bones; }

	KeyFrame* FindKeyFrame(float time){
		for (unsigned int i = 0; i < keyFrames.size(); ++i){
			if (keyFrames[i]->time == time)
				return keyFrames[i];
		}

		return nullptr;
	}

	void CalculateDuration(){
		if (keyFrames.size() > 0)
			duration = keyFrames.back()->time;
		else
			duration = 0.0f;
	}

private:
	std::string name;
	float duration;

	std::vector< KeyFrame* > keyFrames;

	std::vector<Bone*> bones;

	void Destroy() {
		unsigned int size = (unsigned int)keyFrames.size();
		for (unsigned int i = 0; i < size; ++i) {
			delete keyFrames[i];
		}

		keyFrames.clear();
	}
};