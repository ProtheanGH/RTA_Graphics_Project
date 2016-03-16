#include "Camera.h"

// ===== Constructor / Destructor ===== //
Camera::Camera()
{
	m_Transform = XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, -2, 1);
	XMMATRIX matrix = XMLoadFloat4x4(&m_Transform);
	XMVECTOR determinant = XMMatrixDeterminant(matrix);
	matrix = XMMatrixInverse(&determinant, matrix);
	XMStoreFloat4x4(&m_Transform, matrix);

	m_CursorPosition.x = -1;
	m_MovementSpeed = 12.0;
	m_FastMovementSpeed = 50.0f;
	m_RotationSpeed = 0.5;
}

Camera::~Camera()
{
	
}
// ==================================== //

// ===== Interface ===== //
void Camera::Update(float _deltaTime)
{
	XMMATRIX matrix = XMLoadFloat4x4(&m_Transform);
	XMVECTOR determinant = XMMatrixDeterminant(matrix);
	matrix = XMMatrixInverse(&determinant, matrix);

	// === Forward / Backward Movement (Z-Axis)
	if (GetAsyncKeyState('W')) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			matrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, _deltaTime * m_FastMovementSpeed), matrix);
		}
		else {
			matrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, _deltaTime * m_MovementSpeed), matrix);
		}
	}
	else if (GetAsyncKeyState('S')) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			matrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, _deltaTime * -m_FastMovementSpeed), matrix);
		}
		else {
			matrix = XMMatrixMultiply(XMMatrixTranslation(0, 0, _deltaTime * -m_MovementSpeed), matrix);
		}
	}

	// === Sidewards Movement (X-Axis)
	if (GetAsyncKeyState('A')) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			matrix = XMMatrixMultiply(XMMatrixTranslation(_deltaTime * -m_FastMovementSpeed, 0, 0), matrix);
		}
		else {
			matrix = XMMatrixMultiply(XMMatrixTranslation(_deltaTime * -m_MovementSpeed, 0, 0), matrix);
		}
	}
	else if (GetAsyncKeyState('D')) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			matrix = XMMatrixMultiply(XMMatrixTranslation(_deltaTime * m_FastMovementSpeed, 0, 0), matrix);
		}
		else {
			matrix = XMMatrixMultiply(XMMatrixTranslation(_deltaTime * m_MovementSpeed, 0, 0), matrix);
		}
	}

	// === Fly Up / Down (Y-Axis)
	if (GetAsyncKeyState('E')) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			matrix = XMMatrixMultiply(XMMatrixTranslation(0, _deltaTime * m_FastMovementSpeed, 0), matrix);
		}
		else {
			matrix = XMMatrixMultiply(XMMatrixTranslation(0, _deltaTime * m_MovementSpeed, 0), matrix);
		}
	}
	else if (GetAsyncKeyState('Q')) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			matrix = XMMatrixMultiply(XMMatrixTranslation(0, _deltaTime * -m_FastMovementSpeed, 0), matrix);
		}
		else {
			matrix = XMMatrixMultiply(XMMatrixTranslation(0, _deltaTime * -m_FastMovementSpeed, 0), matrix);
		}
	}

	// Camera Rotation
	if (GetAsyncKeyState(VK_RBUTTON)) {
		if (m_CursorPosition.x == -1)
			GetCursorPos(&m_CursorPosition);
		// == Get the New Cursor Position
		POINT newCursorPos;
		GetCursorPos(&newCursorPos);
		float distance;
		// === Left / Right Rotation
		if (newCursorPos.x != m_CursorPosition.x) {
			XMVECTOR position = matrix.r[3];
			matrix = XMMatrixMultiply(matrix, XMMatrixTranslation(0, 0, 0));
			distance = (float)m_CursorPosition.x - (float)newCursorPos.x;
			matrix = XMMatrixMultiply(matrix, XMMatrixRotationY(-distance * 0.0174532925f));
			matrix.r[3] = position;
		}
		// === Up / Down Rotation
		if (newCursorPos.y != m_CursorPosition.y) {
			distance = (float)m_CursorPosition.y - (float)newCursorPos.y;
			matrix = XMMatrixMultiply(XMMatrixRotationX(-distance * 0.0174532925f), matrix);
		}

		m_CursorPosition = newCursorPos;
	}
	else {
		m_CursorPosition.x = -1;
	}

	determinant = XMMatrixDeterminant(matrix);
	matrix = XMMatrixInverse(&determinant, matrix);
	XMStoreFloat4x4(&m_Transform, matrix);
}
// ===================== //

// === Accessors === //
XMFLOAT3 Camera::GetPosition()
{
	XMMATRIX matrix = XMLoadFloat4x4(&m_Transform);
	XMVECTOR determinant = XMMatrixDeterminant(matrix);
	matrix = XMMatrixInverse(&determinant, matrix);

	XMFLOAT4X4 worldPos;
	XMStoreFloat4x4(&worldPos, matrix);

	return XMFLOAT3(worldPos._41, worldPos._42, worldPos._43);
}
// ================= //