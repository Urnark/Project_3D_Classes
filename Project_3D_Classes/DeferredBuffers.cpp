#include "DeferredBuffers.h"

DeferredBuffers::DeferredBuffers()
{
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		this->renderTargetTextureArray[i] = nullptr;
		this->renderTargetViewArray[i] = nullptr;
		this->shaderResourceViewArray[i] = nullptr;
	}

	this->depthStencilBuffer = nullptr;
	this->depthStencilView = nullptr;
}

DeferredBuffers::DeferredBuffers(const DeferredBuffers &)
{
}

DeferredBuffers::~DeferredBuffers()
{
}

bool DeferredBuffers::initialize(ID3D11Device * device, int textureWidth, int textureHeight/*, int screenDepth, int screenNear*/)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	int i;

	// Store the width and height of the render texture.
	this->textureWidth = textureWidth;
	this->textureHeight = textureHeight;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target textures.
	for (i = 0; i<BUFFER_COUNT; i++)
	{
		result = device->CreateTexture2D(&textureDesc, NULL, &this->renderTargetTextureArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target views.
	for (i = 0; i<BUFFER_COUNT; i++)
	{
		result = device->CreateRenderTargetView(this->renderTargetTextureArray[i], &renderTargetViewDesc, &this->renderTargetViewArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource views.
	for (i = 0; i<BUFFER_COUNT; i++)
	{
		result = device->CreateShaderResourceView(this->renderTargetTextureArray[i], &shaderResourceViewDesc, &this->shaderResourceViewArray[i]);
		if (FAILED(result))
		{
			return false;
		}
	}

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
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
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &this->depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(this->depthStencilBuffer, &depthStencilViewDesc, &this->depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the viewport for rendering.
	this->viewport.Width = (float)textureWidth;
	this->viewport.Height = (float)textureHeight;
	this->viewport.MinDepth = 0.0f;
	this->viewport.MaxDepth = 1.0f;
	this->viewport.TopLeftX = 0.0f;
	this->viewport.TopLeftY = 0.0f;

	return true;
}

void DeferredBuffers::shutdown()
{
	if (this->depthStencilView)
	{
		this->depthStencilView->Release();
		this->depthStencilView = nullptr;
	}

	if (this->depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		this->depthStencilBuffer = nullptr;
	}

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		if (this->shaderResourceViewArray[i])
		{
			this->shaderResourceViewArray[i]->Release();
			this->shaderResourceViewArray[i] = nullptr;
		}

		if (this->renderTargetViewArray[i])
		{
			this->renderTargetViewArray[i]->Release();
			this->renderTargetViewArray[i] = nullptr;
		}

		if (this->renderTargetTextureArray[i])
		{
			this->renderTargetTextureArray[i]->Release();
			this->renderTargetTextureArray[i] = nullptr;
		}
	}

}

void DeferredBuffers::setRenderTargets(ID3D11DeviceContext * deviceContext)
{
	// Bind the render target view array and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(BUFFER_COUNT, this->renderTargetViewArray, this->depthStencilView);

	// Set the viewport.
	deviceContext->RSSetViewports(1, &this->viewport);
}

void DeferredBuffers::clearRenderTargets(ID3D11DeviceContext * deviceContext, float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the render target buffers.
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		deviceContext->ClearRenderTargetView(this->renderTargetViewArray[i], color);
	}

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(this->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

ID3D11ShaderResourceView * DeferredBuffers::getShaderResourceView(int view)
{
	return this->shaderResourceViewArray[view];
}
