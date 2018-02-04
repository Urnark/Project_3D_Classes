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
#include "DeferredBuffers.h"
#include "DeferredShader.h"
#include "LightShader.h"
#include "Light.h"
#include "orthowindowclass.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

using namespace DirectX;

class Graphics
{
private:
	DirectX3D* directX3D;
	Model* model;
	Shader* shader;
	Camera* camera;
	Player* player;
	DeferredBuffers* deferredBuffers;
	DeferredShader* deferredShader;
	LightShader* lightShader;
	Light* light;
	OrthoWindow* fullScreenWindow;

	bool render(float dt);
	bool renderSceneToTexture( float dt);
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool initialize(int screenWidth, int screenHeight, HWND window);
	void shutdown();
	bool frame(float dt, Input* input, float screenWidth, float screenHeight);

};

#endif