#pragma once
#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

class Renderer;

class Camera
{
private:
	// === Members
	XMFLOAT4X4 m_Transform;
	POINT m_CursorPosition;
	float m_MovementSpeed;
	float m_FastMovementSpeed;
	float m_RotationSpeed;

	void SetCursorMiddle();
	void MouseLook(DirectX::XMMATRIX& _matrix);
	void HandleCursorVisible(const bool _buttonDown);

public:
	// === Constructor / Destructor === //
	Camera();
	~Camera();
	// ================================ //

	// === Interface === //
	void Update(float _deltaTime);
	// ================= //

	// === Accessors === //
	inline XMFLOAT4X4 GetViewMatrix() {
		return m_Transform;
	}
	XMFLOAT3 GetPosition();
	// ================= //
};

