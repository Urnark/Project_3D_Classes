#ifndef LIGHT_H
#define LIGHT_H

#include <DirectXMath.h>
using namespace DirectX;

class Light
{
private:
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 position;

public:
	Light();
	Light(const Light& other);
	~Light();

	void setAmbientColor(float red, float green, float blue, float alpha);
	void setDiffuseColor(float red, float green, float blue, float alpha);
	void setPosition(float x, float y, float z);

	XMVECTOR getAmbientColor();
	XMVECTOR getDiffuseColor();
	XMVECTOR getPosition();
};

#endif