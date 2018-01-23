#ifndef DIRECTX3D_H
#define DIRECTX3D_H

#include <d3d11.h>
#include <DirectXMath.h>

class DirectX3D
{
private:

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthDisabledStencilState;
	ID3D11DepthStencilView* depthStencilView;
	DirectX::XMMATRIX projectionMatrix;
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX orthoMatrix;

public:
	DirectX3D();
	DirectX3D(const DirectX3D&);
	~DirectX3D();

	bool initialize(int screenWidth, int screenHeight, HWND window, float screenDepth, float screenNear);
	void shutdown();

	void beginScene(float red, float green, float blue, float alpha);
	void endScene();

	void turnZBufferOn();
	void turnZBufferOff();

	// Getters
	ID3D11Device* DirectX3D::getDevice();
	ID3D11DeviceContext* DirectX3D::getDeviceContext();
	DirectX::XMMATRIX getProjectionMatrix();
	DirectX::XMMATRIX getWorldMatrix();
	DirectX::XMMATRIX getOrthoMatrix();
};

#endif