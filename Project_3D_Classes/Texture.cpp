#include "Texture.h"

Texture::Texture()
{
	this->texture = nullptr;
	this->texture2D = nullptr;
}

Texture::Texture(const Texture &)
{
}

Texture::~Texture()
{
}

bool Texture::initialize(ID3D11Device * device)
{
	// Create texture
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = BTH_IMAGE_WIDTH;
	textureDesc.Height = BTH_IMAGE_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(textureData));
	textureData.pSysMem = BTH_IMAGE_DATA;
	textureData.SysMemPitch = BTH_IMAGE_WIDTH * 4;
	textureData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateTexture2D(&textureDesc, &textureData, &this->texture2D);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
	textureViewDesc.Format = textureDesc.Format;
	textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	textureViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	textureViewDesc.Texture2D.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(this->texture2D, &textureViewDesc, &this->texture);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void Texture::shutdown()
{
	if (this->texture2D)
	{
		this->texture2D->Release();
		this->texture2D = nullptr;
	}
	if (this->texture)
	{
		this->texture->Release();
		this->texture = nullptr;
	}
}

ID3D11ShaderResourceView * Texture::getTexture()
{
	return this->texture;
}
