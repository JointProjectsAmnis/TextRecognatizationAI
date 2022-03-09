#pragma once
#include "Graphics.h"

class Texture
{
public:

	Graphics* graphics;

	int width;
	int height;
	int channels;
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* shaderResource = nullptr;;

	Texture(Graphics* graphics, int width, int height, int channels, void* data);
	~Texture();

	void SetTexture(int width, int height, int channels, void* data);
	void Bind(UINT slot);
};

