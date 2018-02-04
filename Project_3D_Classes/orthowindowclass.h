#ifndef ORTHOWINDOW_H
#define ORTHOWINDOW_H

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class OrthoWindow
{
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount, indexCount;

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	bool initializeBuffers(ID3D11Device* device, int windowWidth, int windowHeight);
	void shutdownBuffers();
	void renderBuffers(ID3D11DeviceContext* deviceContext);
public:
	OrthoWindow();
	OrthoWindow(const OrthoWindow&);
	~OrthoWindow();

	bool initialize(ID3D11Device* device, int windowWidth, int windowHeight);
	void shutdown();
	void render(ID3D11DeviceContext* deviceContext);

	int getIndexCount();

};

#endif