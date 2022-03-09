#pragma once
#include "Graphics.h"

class Sampler
{
public:

	Graphics* graphics;

	ID3D11SamplerState* sampler;

	Sampler(Graphics* graphics, D3D11_TEXTURE_ADDRESS_MODE type = D3D11_TEXTURE_ADDRESS_WRAP);

	void Bind(UINT slot);

};



