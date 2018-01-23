#include "Camera.h"

Camera::Camera()
{
	this->posX = 0.0f;
	this->posY = 0.0f;
	this->posZ = 0.0f;
	
	this->rotX = 0.0f;
	this->rotY = 0.0f;
	this->rotZ = 0.0f;
}

Camera::Camera(const Camera &)
{
}

Camera::~Camera()
{
}

void Camera::setPosition(float x, float y, float z)
{
	this->posX = x;
	this->posY = y;
	this->posZ = z;
}

void Camera::setRotation(float x, float y, float z)
{
	this->rotX = x;
	this->rotY = y;
	this->rotZ = z;
}

DirectX::XMVECTOR Camera::getPosition()
{
	DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(this->posX, this->posY, this->posZ);
	return DirectX::XMLoadFloat3(&pos);
}

DirectX::XMVECTOR Camera::getRotation()
{
	DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(this->rotX, this->rotY, this->rotZ);
	return  DirectX::XMLoadFloat3(&rot);
}

void Camera::render()
{
	DirectX::XMFLOAT3 up, position, lookAt;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = this->posX;
	position.y = this->posY;
	position.z = this->posZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = this->rotX * 0.0174532925f;
	yaw = this->rotY * 0.0174532925f;
	roll = this->rotZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	DirectX::XMStoreFloat3(&lookAt, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&lookAt), rotationMatrix));
	DirectX::XMStoreFloat3(&up, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&up), rotationMatrix));

	// Translate the rotated camera position to the location of the viewer.
	lookAt.x = position.x + lookAt.x;
	lookAt.y = position.y + lookAt.y;
	lookAt.z = position.z + lookAt.z;

	// Finally create the view matrix from the three updated vectors.
	this->viewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&lookAt), DirectX::XMLoadFloat3(&up));

	return;
}

void Camera::getViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
	viewMatrix = this->viewMatrix;
}
