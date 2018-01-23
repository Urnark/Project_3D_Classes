#ifndef MODEL_H
#define MODEL_H

#include <d3d11.h>
#include <DirectXMath.h>

class Model
{
private:
	ID3D11Buffer* vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
public:
	Model();
	Model(const Model&);
	~Model();

	bool initialize(ID3D11Device* device);
	void shutdown();
	void render(ID3D11DeviceContext* deviceContext);

	int getIndexCount();
};

#endif