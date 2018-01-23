#include "DirectX3D.h"

DirectX3D::DirectX3D()
{
	this->device = nullptr;
	this->deviceContext = nullptr;
	this->swapChain = nullptr;
	this->renderTargetView = nullptr;
	this->depthStencilBuffer = nullptr;
	this->depthStencilState = nullptr;
	this->depthStencilView = nullptr;
}

DirectX3D::DirectX3D(const DirectX3D &)
{
}

DirectX3D::~DirectX3D()
{
}

bool DirectX3D::initialize(int screenWidth, int screenHeight, HWND window, float screenDepth, float screenNear)
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Create the swap chain, device and device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, 
		D3D11_SDK_VERSION, &swapChainDesc, &this->swapChain, &this->device, NULL, &this->deviceContext);
	if (FAILED(result))
		return false;

	// Get the pointer to the back buffer.
	ID3D11Texture2D* backBufferPtr;
	result = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
		return false;

	// Create the render target view with the back buffer pointer.
	result = this->device->CreateRenderTargetView(backBufferPtr, NULL, &this->renderTargetView);
	if (FAILED(result))
		return false;

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = this->device->CreateTexture2D(&depthBufferDesc, NULL, &this->depthStencilBuffer);
	if (FAILED(result))
		return false;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = this->device->CreateDepthStencilState(&depthStencilDesc, &this->depthStencilState);
	if (FAILED(result))
		return false;

	// Set the depth stencil state.
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState, 1);

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = this->device->CreateDepthStencilView(this->depthStencilBuffer, &depthStencilViewDesc, &this->depthStencilView);
	if (FAILED(result))
		return false;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	this->deviceContext->OMSetRenderTargets(1, &this->renderTargetView, this->depthStencilView);

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	this->deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)DirectX::XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	
	// Initialize the world matrix to the identity matrix.
	this->worldMatrix = DirectX::XMMatrixIdentity();
	
	// Create an orthographic projection matrix for 2D rendering.
	this->orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// depth stencil state which turns off the Z buffer for 2D rendering.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = this->device->CreateDepthStencilState(&depthDisabledStencilDesc, &this->depthDisabledStencilState);
	if (FAILED(result))
		return false;

	return true;
}

void DirectX3D::shutdown()
{
	if (this->device)
	{
		this->device->Release();
		this->device = nullptr;
	}

	if (this->deviceContext)
	{
		this->deviceContext->Release();
		this->deviceContext = nullptr;
	}

	if (this->renderTargetView)
	{
		this->renderTargetView->Release();
		this->renderTargetView = nullptr;
	}

	if (this->depthStencilBuffer)
	{
		this->depthStencilBuffer->Release();
		this->depthStencilBuffer = nullptr;
	}

	if (this->depthStencilState)
	{
		this->depthStencilState->Release();
		this->depthStencilState = nullptr;
	}

	if (this->depthStencilView)
	{
		this->depthStencilView->Release();
		this->depthStencilView = nullptr;
	}

	if (this->swapChain)
	{
		this->swapChain->SetFullscreenState(false, nullptr);
		this->swapChain->Release();
		this->swapChain = nullptr;
	}
}

void DirectX3D::beginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	this->deviceContext->ClearRenderTargetView(this->renderTargetView, color);

	// Clear the depth buffer.
	this->deviceContext->ClearDepthStencilView(this->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectX3D::endScene()
{
	// Present the back buffer to the screen since rendering is complete.
	this->swapChain->Present(0, 0);
}

void DirectX3D::turnZBufferOn()
{
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState, 1);
}

void DirectX3D::turnZBufferOff()
{
	this->deviceContext->OMSetDepthStencilState(this->depthDisabledStencilState, 1);
}

ID3D11Device * DirectX3D::getDevice()
{
	return this->device;
}

ID3D11DeviceContext * DirectX3D::getDeviceContext()
{
	return this->deviceContext;
}

DirectX::XMMATRIX DirectX3D::getProjectionMatrix()
{
	return this->projectionMatrix;
}

DirectX::XMMATRIX DirectX3D::getWorldMatrix()
{
	return this->worldMatrix;
}

DirectX::XMMATRIX DirectX3D::getOrthoMatrix()
{
	return this->orthoMatrix;
}
