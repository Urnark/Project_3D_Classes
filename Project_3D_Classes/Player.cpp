#include "Player.h"

void Player::movment(float dt, Input * input)
{
	XMVECTOR f = XMLoadFloat3(&this->camera->getForward());
	XMVECTOR r = XMLoadFloat3(&this->camera->getRight());

	if (input->isKeyPressed(DIK_A))
	{
		this->position -= r * SPEED * dt;
	}

	if (input->isKeyPressed(DIK_D))
	{
		this->position += r * SPEED * dt;
	}

	if (input->isKeyPressed(DIK_W))
	{
		this->position += f * SPEED * dt;
	}

	if (input->isKeyPressed(DIK_S))
	{
		this->position -= f * SPEED * dt;
	}
}

void Player::cameraMovment(Input* input, int screenWidth, int screenHeight)
{
	static int mouseX = 0, mouseY = 0;

	// Get the location of the mouse from the input object
	input->getMouseLocation(mouseX, mouseY);
	screenWidth /= 2;
	screenHeight /= 2;

	// horizontal rotation (yaw)
	this->yaw += (mouseX - (screenWidth)) * 0.2;
	// vertical rotation (pitch)
	this->pitch += (mouseY - screenHeight) * 0.2;

	// Set the mouse location to the center of the screen.
	input->setMouseLocation(screenWidth, screenHeight);
}

Player::Player()
{
	this->pitch = 0.0f;
	this->yaw = 0.0f;
	this->roll = 0.0f;
	this->camera = nullptr;
}

Player::Player(const Player &)
{
}

Player::~Player()
{
}

bool Player::initialize(Camera * camera)
{
	if (camera == nullptr)
	{
		return false;
	}

	this->camera = camera;
	this->position = camera->getPosition();

	return true;
}

void Player::shutdown()
{
	this->camera = nullptr;
}

void Player::frame(float dt, Input* input, int screenWidth, int screenHeight)
{
	this->position = camera->getPosition();


	this->movment(dt, input);
	this->cameraMovment(input, screenWidth, screenHeight);


	XMFLOAT3 v;
	XMStoreFloat3(&v, this->position);
	this->camera->setPosition(v.x, v.y, v.z);
	this->camera->setRotation(this->pitch, this->yaw, this->roll);
}
