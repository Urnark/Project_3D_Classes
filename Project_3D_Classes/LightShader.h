#ifndef LIGHT_SHADER_H
#define LIGHT_SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;
using namespace DirectX;

class LightShader
{
private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMVECTOR lightPosition;
		XMVECTOR ambientColor;
		XMVECTOR diffuseColor;
	};

	bool initializeShader(ID3D11Device* device, HWND window, WCHAR* vsFilename, WCHAR* psFilename);
	void shutdownShader();
	void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND window, WCHAR* shaderFilename);

	bool setShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, 
		ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* positionTexture, XMVECTOR &lightPosition, XMVECTOR &ambientColor, XMVECTOR &diffuseColor);
	void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);
public:
	LightShader();
	LightShader(const LightShader&);
	~LightShader();

	bool initialize(ID3D11Device* device, HWND window);
	void shutdown();
	bool render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix, 
		ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* positionTexture, XMVECTOR &lightPosition, XMVECTOR &ambientColor, XMVECTOR &diffuseColor);
};

#endif