#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>

class Animation{

public:

	struct Key{
		float time;
		DirectX::XMFLOAT4 translation, rotation, scale;
	};

	struct KeyFrame{
		std::string name;
		std::vector<Key > keys;
	};

public:
	Animation();
	~Animation();

	inline float GetDuration() const { return duration; }
	inline std::string& GetName(){ return name; }
	inline std::vector<KeyFrame>& GetKeyFrames(){ return keyFrames; }

	void CalculateDuration(){
		duration = keyFrames[0].keys.back().time;
	}
	
private:
	std::string name;
	float duration;

	std::vector< KeyFrame > keyFrames;
};