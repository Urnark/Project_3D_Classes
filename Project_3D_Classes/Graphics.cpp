#include "Graphics.h"

Graphics::Graphics()
{
	this->directX3D = nullptr;
	this->model = nullptr;
	this->shader = nullptr;
	this->camera = nullptr;
	this->deferredBuffers = nullptr;
	this->deferredShader = nullptr;
	this->lightShader = nullptr;
	this->light = nullptr;
	this->fullScreenWindow = nullptr;
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
	this->camera->render();
	this->camera->renderBaseViewMatrix();

	this->player = new Player();
	if (!this->player)
	{
		return false;
	}
	this->player->initialize(this->camera);

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

	this->light = new Light();
	if (!this->light)
	{
		return false;
	}
	this->light->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	this->light->setPosition(0.0f, 0.0f, -11.0f);

	this->fullScreenWindow = new OrthoWindow();
	if (!this->fullScreenWindow)
	{
		return false;
	}
	if (!this->fullScreenWindow->initialize(this->directX3D->getDevice(), screenWidth, screenHeight))
	{
		return false;
	}

	this->deferredBuffers = new DeferredBuffers();
	if (!this->deferredBuffers)
	{
		return false;
	}
	if (!this->deferredBuffers->initialize(this->directX3D->getDevice(), screenWidth, screenHeight))
	{
		return false;
	}

	this->deferredShader = new DeferredShader();
	if (!this->deferredShader)
	{
		return false;
	}
	if (!this->deferredShader->initialize(this->directX3D->getDevice(), window))
	{
		return false;
	}

	this->lightShader = new LightShader();
	if (!this->lightShader)
	{
		return false;
	}
	if (!this->lightShader->initialize(this->directX3D->getDevice(), window))
	{
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

	if (this->deferredBuffers)
	{
		this->deferredBuffers->shutdown();
		delete this->deferredBuffers;
		this->deferredBuffers = nullptr;
	}

	if (this->deferredShader)
	{
		this->deferredShader->shutdown();
		delete this->deferredShader;
		this->deferredShader = nullptr;
	}

	if (this->lightShader)
	{
		this->lightShader->shutdown();
		delete this->lightShader;
		this->lightShader = nullptr;
	}

	if (this->fullScreenWindow)
	{
		this->fullScreenWindow->shutdown();
		delete this->fullScreenWindow;
		this->fullScreenWindow = nullptr;
	}

	if (this->light)
	{
		delete this->light;
		this->light = nullptr;
	}

	// Release the model object.
	if (this->model)
	{
		this->model->shutdown();
		delete this->model;
		this->model = nullptr;
	}

	// Release the player object.
	if (this->player)
	{
		this->player->shutdown();
		delete this->player;
		this->player = nullptr;
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

bool Graphics::frame(float dt, Input* input, float screenWidth, float screenHeight)
{
	//string s = "Right: (px: " + std::to_string(r1.x) + ", py: " + std::to_string(r1.y) + ", pz: " + std::to_string(r1.z) + ")\n";
	//OutputDebugStringA(s.c_str());

	this->player->frame(dt, input, screenWidth, screenHeight);

	if (!render(dt))
	{
		return false;
	}

	return true;
}

bool Graphics::render(float dt)
{
	/*DirectX::XMMATRIX viewMatrix, projectionMatrix, worldMatrix;

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
	if (!this->shader->render(this->directX3D->getDeviceContext(), this->model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, this->model->getTexture()))
	{
		return false;
	}

	// Present the rendered scene to the screen.
	this->directX3D->endScene();
	*/
	
	DirectX::XMMATRIX viewMatrix, orthoMatrix, worldMatrix;

	// Render the scene to the render buffers.
	if (!renderSceneToTexture(dt))
	{
		return false;
	}

	// Clear the scene.
	this->directX3D->beginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the matrices.
	worldMatrix = this->directX3D->getWorldMatrix();
	this->camera->getBaseViewMatrix(viewMatrix);
	orthoMatrix = this->directX3D->getOrthoMatrix();

	// Turn off the Z buffer to begin all 2D rendering.
	this->directX3D->turnZBufferOff();

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->fullScreenWindow->render(this->directX3D->getDeviceContext());

	// Render the full screen ortho window using the deferred light shader and the render buffers.
	this->lightShader->render(this->directX3D->getDeviceContext(), this->fullScreenWindow->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		this->deferredBuffers->getShaderResourceView(0), this->deferredBuffers->getShaderResourceView(1), this->deferredBuffers->getShaderResourceView(2),
		this->light->getPosition(), this->light->getAmbientColor(), this->light->getDiffuseColor());

	// Turn the Z buffer back on now that all 2D rendering has completed.
	this->directX3D->turnZBufferOn();

	// Present the rendered scene to the screen.
	this->directX3D->endScene();

	return true;
}

bool Graphics::renderSceneToTexture(float dt)
{
	DirectX::XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
	//Set the deferred render to texture buffers as the render target and then clear them before rendering.

	// Set the render buffers to be the render target.
	this->deferredBuffers->setRenderTargets(this->directX3D->getDeviceContext());

	// Clear the render buffers.
	this->deferredBuffers->clearRenderTargets(this->directX3D->getDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	//Render the scene of the spinning cube using the deferred shader.

	// Generate the view matrix based on the camera's position.
	this->camera->render();

	// Get the matrices from the camera and d3d objects.
	worldMatrix = this->directX3D->getWorldMatrix();
	this->camera->getViewMatrix(viewMatrix);
	projectionMatrix = this->directX3D->getProjectionMatrix();

	// Update the rotation variable each frame.
	static float rotation = 0.0f;
	rotation += XM_PI * 0.2f * dt;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Rotate the world matrix by the rotation value so that the cube will spin.
	worldMatrix = XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	this->model->render(this->directX3D->getDeviceContext());

	// Render the model using the deferred shader.
	this->deferredShader->render(this->directX3D->getDeviceContext(), this->model->getIndexCount(), worldMatrix, viewMatrix, projectionMatrix, this->model->getTexture());
	//Set the render target back to the back buffer.

	// Reset the render target back to the original back buffer and not the render buffers anymore.
	this->directX3D->setBackBufferRenderTarget();

	// Reset the viewport back to the original.
	this->directX3D->resetViewport();

	return true;
}