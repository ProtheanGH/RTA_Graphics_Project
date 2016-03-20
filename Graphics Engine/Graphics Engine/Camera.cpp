#include "Camera.h"
#include "Renderer.h"

#ifndef ROTATION_MODIFIER
#define ROTATION_MODIFIER	0.2f
#endif

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
void Camera::Update(HWND _hwnd, float _deltaTime)
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

	// === Camera Rotation
	static bool resetMouse = true;
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		if (resetMouse == true)
		{
			SetCursorMiddle(_hwnd);
			resetMouse = false;
		}
		else
		{
			MouseLook(_hwnd, matrix);
		}
	}
	else
	{
		resetMouse = true;
	}
	// ===

	determinant = XMMatrixDeterminant(matrix);
	matrix = XMMatrixInverse(&determinant, matrix);
	XMStoreFloat4x4(&m_Transform, matrix);
}

void Camera::SetCursorMiddle(HWND _hwnd)
{
	D3D11_VIEWPORT view = Renderer::GetInstance()->GetViewport();
	POINT temp;
	temp.x = (int)view.TopLeftX + ((int)view.Width / 2);
	temp.y = (int)view.TopLeftY + ((int)view.Height / 2);
	ClientToScreen(_hwnd, &temp);
	SetCursorPos(temp.x, temp.y);
}

void Camera::MouseLook(HWND _hwnd, DirectX::XMMATRIX& _matrix)
{
	D3D11_VIEWPORT view = Renderer::GetInstance()->GetViewport();

	// GetThe center of the client window
	POINT centerClientWindow;
	centerClientWindow.x = (int)view.TopLeftX + ((int)view.Width / 2);
	centerClientWindow.y = (int)view.TopLeftY + ((int)view.Height / 2);

	// Convert mouse to client space
	POINT cursorPositionInScreenSpace;
	GetCursorPos(&cursorPositionInScreenSpace);
	ScreenToClient(_hwnd, &cursorPositionInScreenSpace);

	// Convert the difference into radians
	float radX = DirectX::XMConvertToRadians((float)cursorPositionInScreenSpace.x - (float)centerClientWindow.x);
	float radY = DirectX::XMConvertToRadians((float)cursorPositionInScreenSpace.y - (float)centerClientWindow.y);
	radX *= ROTATION_MODIFIER;
	radY *= ROTATION_MODIFIER;

	SetCursorMiddle(_hwnd);

	XMVECTOR localPosition = _matrix.r[3];

	_matrix = XMMatrixMultiply( DirectX::XMMatrixRotationX( radY ), _matrix );
	_matrix = XMMatrixMultiply( _matrix, DirectX::XMMatrixRotationY( radX ) );

	_matrix.r[3] = localPosition;
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