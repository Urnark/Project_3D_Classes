#ifndef COLORSHADER_H
#define COLORSHADER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;

class Shader
{
private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	bool initializeShader(ID3D11Device* device, HWND window, WCHAR* vsFilename, WCHAR* psFilename);
	void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND window, WCHAR* shaderFilename);

	bool setShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX &worldMatrix, DirectX::XMMATRIX &viewMatrix, DirectX::XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture);
	void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);
public:
	Shader();
	Shader(const Shader&);
	~Shader();
	
	bool initialize(ID3D11Device* device, HWND window);
	void shutdown();
	bool render(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX &worldMatrix, DirectX::XMMATRIX &viewMatrix, DirectX::XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture);
};

#endif