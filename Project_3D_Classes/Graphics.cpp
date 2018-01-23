#include "Graphics.h"

Graphics::Graphics()
{
	this->directX3D = nullptr;
	this->model = nullptr;
	this->shader = nullptr;
	this->camera = nullptr;
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


	// Create the camera object.
	this->camera = new Camera();
	if (!this->camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	this->camera->setPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	this->model = new Model();
	if (!this->model)
	{
		return false;
	}

	// Initialize the model object.
	if (!this->model->initialize(this->directX3D->getDevice()))
	{
		MessageBox(window, "Could not initialize the model object.", "Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	this->shader = new Shader();
	if (!this->shader)
	{
		return false;
	}

	// Initialize the color shader object.
	if (!this->shader->initialize(this->directX3D->getDevice(), window))
	{
		MessageBox(window, "Could not initialize the color shader object.", "Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::shutdown()
{
	// Release the color shader object.
	if (this->shader)
	{
		this->shader->shutdown();
		delete this->shader;
		this->shader = nullptr;
	}

	// Release the model object.
	if (this->model)
	{
		this->model->shutdown();
		delete this->model;
		this->model = nullptr;
	}

	// Release the camera object.
	if (this->camera)
	{
		delete this->camera;
		this->camera = nullptr;
	}


	if (this->directX3D)
	{
		this->directX3D->shutdown();
		delete this->directX3D;
		this->directX3D = nullptr;
	}
}

bool Graphics::frame(Input *input)
{
	if (!render())
	{
		return false;
	}

	return true;
}

bool Graphics::render()
{
	DirectX::XMMATRIX viewMatrix, projectionMatrix, worldMatrix;

	// Clear the buffers to begin the scene.
	this->directX3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	this->camera->render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	this->camera->getViewMatrix(viewMatrix);
	worldMatrix = this->directX3D->getWorldMatrix();
	projectionMatrix = this->directX3D->getProjectionMatrix();

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->model->render(this->directX3D->getDeviceContext());

	// Render the model using the color shader.
	if (!this->shader->render(this->directX3D->getDeviceContext(), this->model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}

	// Present the rendered scene to the screen.
	this->directX3D->endScene();

	return true;
}