#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <windows.h>
#include "DirectX3D.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
private:
	bool render();

	DirectX3D* directX3D;
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool initialize(int screenWidth, int screenHeight, HWND window);
	void shutdown();
	bool frame();

};

#endif