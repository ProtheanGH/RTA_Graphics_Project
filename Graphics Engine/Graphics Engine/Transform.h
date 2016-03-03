#pragma once

#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")

class Transform{

private:
	DirectX::XMFLOAT4X4 localMatrix;

public:
	Transform() = default;
	~Transform() = default;

	inline DirectX::XMFLOAT4X4& GetLocalMatrix() { return localMatrix; }
};