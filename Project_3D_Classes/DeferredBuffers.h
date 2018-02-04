#ifndef DEFERRED_BUFFERS_H
#define DEFERRED_BUFFERS_H

#include <d3d11.h>

const int BUFFER_COUNT = 3;

class DeferredBuffers
{
private:
	int textureWidth, textureHeight;

	ID3D11Texture2D* renderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* renderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* shaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT viewport;
public:
	DeferredBuffers();
	DeferredBuffers(const DeferredBuffers&);
	~DeferredBuffers();

	bool initialize(ID3D11Device* device, int textureWidth, int textureHeight/*, int screenDepth, int screenNear*/);
	void shutdown();

	void setRenderTargets(ID3D11DeviceContext* deviceContext);
	void clearRenderTargets(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha);

	ID3D11ShaderResourceView* getShaderResourceView(int view);
};

#endif