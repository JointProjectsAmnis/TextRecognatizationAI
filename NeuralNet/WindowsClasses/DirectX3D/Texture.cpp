#include "Texture.h"

Texture::Texture(Graphics* graphics, int width, int height, int channels, void* data)
{
	this->width = width;
	this->height = height;
	this->channels;

	this->graphics = graphics;

	SetTexture(width, height, channels, data);

}

Texture::~Texture()
{
	if (texture)
	{
		texture->Release();
		texture = nullptr;
	}

	if (shaderResource)
	{
		shaderResource->Release();
		shaderResource = nullptr;
	}
}

void Texture::SetTexture(int width, int height, int channels, void* data)
{
	this->~Texture();

	this->width = width;
	this->height = height;
	this->channels;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;
	sd.SysMemPitch = width * channels;
	HRESULT hr = graphics->device->CreateTexture2D(&desc, &sd, &texture);
	if (FAILED(hr)) throw;

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
	resourceDesc.Format = desc.Format;
	resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MostDetailedMip = 0;
	resourceDesc.Texture2D.MipLevels = 1;

	hr = graphics->device->CreateShaderResourceView(texture, &resourceDesc, &shaderResource);
	if (FAILED(hr)) throw;
}

void Texture::Bind(UINT slot)
{
	graphics->context->PSSetShaderResources(slot, 1, &shaderResource);
}
