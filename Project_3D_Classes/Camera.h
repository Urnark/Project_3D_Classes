#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

class Camera
{
private:
	float posX, posY, posZ;
	float rotX, rotY, rotZ;

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX baseViewMatrix;

	DirectX::XMVECTOR right;
	DirectX::XMVECTOR forward;
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);

	DirectX::XMVECTOR getPosition();
	DirectX::XMVECTOR getRotation();

	void render();
	void getViewMatrix(DirectX::XMMATRIX& viewMatrix);
	void renderBaseViewMatrix();
	void getBaseViewMatrix(DirectX::XMMATRIX& viewMatrix);

	DirectX::XMFLOAT3 getRight();
	DirectX::XMFLOAT3 getForward();
};

#endif