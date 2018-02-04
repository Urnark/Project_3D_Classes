#include "Light.h"

Light::Light()
{
	this->ambientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->diffuseColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->position = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Light::Light(const Light & other)
{
}

Light::~Light()
{
}

void Light::setAmbientColor(float red, float green, float blue, float alpha)
{
	this->ambientColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::setDiffuseColor(float red, float green, float blue, float alpha)
{
	this->diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::setPosition(float x, float y, float z)
{
	this->position = XMFLOAT3(x, y, z);
}

XMVECTOR Light::getAmbientColor()
{
	return XMLoadFloat4(&this->ambientColor);
}

XMVECTOR Light::getDiffuseColor()
{
	return XMLoadFloat4(&this->diffuseColor);
}

XMVECTOR Light::getPosition()
{
	return XMLoadFloat3(&this->position);
}
