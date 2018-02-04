#ifndef MODEL_H
#define MODEL_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"

class Model
{
private:
	ID3D11Buffer* vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;

	Texture* texture;

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

	bool initializeBuffers(ID3D11Device *device);
	bool loadTexture(ID3D11Device *device);
public:
	Model();
	Model(const Model&);
	~Model();

	bool initialize(ID3D11Device* device);
	void shutdown();
	void render(ID3D11DeviceContext* deviceContext);

	int getIndexCount();

	ID3D11ShaderResourceView* getTexture();
};

#endif