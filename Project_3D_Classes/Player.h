#ifndef PLAYER_H
#define PLAYER_H

#include <DirectXMath.h>
#include "Camera.h"
#include "Input.h"

using namespace DirectX;

static const float SPEED = 5.0f;

class Player
{
private:
	XMVECTOR position;
	float yaw, pitch, roll;
	Camera* camera;

	void movment(float dt, Input* input);
	void cameraMovment(Input* input, int screenWidth, int screenHeight);

public:
	Player();
	Player(const Player&);
	~Player();

	bool initialize(Camera* camera);
	void shutdown();
	void frame(float dt, Input* input, int screenWidth, int screenHeight);
};

#endif