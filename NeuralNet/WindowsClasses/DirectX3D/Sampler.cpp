#include "Sampler.h"

Sampler::Sampler(Graphics* graphics, D3D11_TEXTURE_ADDRESS_MODE type)
{
	this->graphics = graphics;

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = type;
	samplerDesc.AddressV = type;
	samplerDesc.AddressW = type;

	HRESULT hr = graphics->device->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(hr)) throw;
}

void Sampler::Bind(UINT slot)
{
	graphics->context->PSSetSamplers(slot, 1, &sampler);
}
