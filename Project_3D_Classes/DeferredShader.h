#ifndef DEFERRED_SHADER_H
#define DEFERRED_SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;
using namespace DirectX;

class DeferredShader
{
private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11SamplerState* sampleStateWrap;
	ID3D11Buffer* matrixBuffer;

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	bool initializeShader(ID3D11Device* device, HWND window, WCHAR* vsFilename, WCHAR* psFilename);
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND window, WCHAR* shaderFilename);

	bool setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture);
	void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);

public:
	DeferredShader();
	DeferredShader(const DeferredShader&);
	~DeferredShader();

	bool initialize(ID3D11Device* device, HWND window);
	void shutdown();
	bool render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture);
};

#endif