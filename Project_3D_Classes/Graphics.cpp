#include "Graphics.h"

Graphics::Graphics()
{
	this->directX3D = nullptr;
}

Graphics::Graphics(const Graphics &)
{
}

Graphics::~Graphics()
{
}

bool Graphics::initialize(int screenWidth, int screenHeight, HWND window)
{
	// Create the Direct3D object.
	this->directX3D = new DirectX3D();
	if (!this->directX3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	if (!this->directX3D->initialize(screenWidth, screenHeight, window, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(window, "Could not initialize Direct3D", "Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::shutdown()
{
	if (this->directX3D)
	{
		this->directX3D->shutDown();
		delete this->directX3D;
		this->directX3D = nullptr;
	}
}

bool Graphics::frame()
{
	if (!render())
	{
		return false;
	}

	return true;
}

bool Graphics::render()
{
	// Clear the buffers to begin the scene.
	this->directX3D->beginScene(0.5f, 0.5f, 0.5f, 1.0f);


	// Present the rendered scene to the screen.
	this->directX3D->endScene();

	return true;
}