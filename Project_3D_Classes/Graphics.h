#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>
#include <string>
#include "DirectX3D.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

using namespace DirectX;

class Graphics
{
private:
	bool render();

	DirectX3D* directX3D;
	Model* model;
	Shader* shader;
	Camera* camera;
	Player* player;
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool initialize(int screenWidth, int screenHeight, HWND window);
	void shutdown();
	bool frame(float dt, Input* input, float screenWidth, float screenHeight);

};

#endif