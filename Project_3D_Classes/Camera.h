#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

class Camera
{
private:
	float posX, posY, posZ;
	float rotX, rotY, rotZ;

	DirectX::XMMATRIX viewMatrix;
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
};

#endif