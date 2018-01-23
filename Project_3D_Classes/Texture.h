#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H

#include <d3d11.h>
#include "bth_image.h"

class Texture
{
private:
	ID3D11Texture2D *texture2D;
	ID3D11ShaderResourceView* texture;

public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool initialize(ID3D11Device *device);
	void shutdown();

	ID3D11ShaderResourceView* getTexture();
};

#endif