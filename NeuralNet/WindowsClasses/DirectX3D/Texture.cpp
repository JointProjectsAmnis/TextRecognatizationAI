#include "Texture.h"

Texture::Texture(Graphics* graphics, int width, int height, int channels, DXGI_FORMAT format, D3D11_USAGE usage, UINT bindFlags, UINT CPUAccessFlags)
{
	this->width = width;
	this->height = height;
	this->channels;

	this->graphics = graphics;

	SetTexture(width, height, channels, format, usage, bindFlags, CPUAccessFlags);
}

Texture::Texture(Graphics* graphics, int width, int height, int channels, void* data, DXGI_FORMAT format, D3D11_USAGE usage, UINT bindFlags, UINT CPUAccessFlags)
{
	this->width = width;
	this->height = height;
	this->channels;

	this->graphics = graphics;

	SetTexture(width, height, channels, data, format, usage, bindFlags, CPUAccessFlags);
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

void Texture::SetTexture(int width, int height, int channels, DXGI_FORMAT format, D3D11_USAGE usage, UINT bindFlags, UINT CPUAccessFlags)
{
	this->~Texture();

	this->width = width;
	this->height = height;
	this->channels = channels;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = usage;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = CPUAccessFlags;
	desc.MiscFlags = 0;

	HRESULT hr = graphics->device->CreateTexture2D(&desc, NULL, &texture);
	if (FAILED(hr)) throw;

	if (usage != D3D11_USAGE_STAGING)
	{

		if (bindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
			resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			resourceDesc.Format = desc.Format;
			resourceDesc.Texture2D.MostDetailedMip = 0;
			resourceDesc.Texture2D.MipLevels = 1;

			hr = graphics->device->CreateShaderResourceView(texture, &resourceDesc, &shaderResource);
			if (FAILED(hr)) throw;
		}

		if (bindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC descView = {};
			descView.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			descView.Format = DXGI_FORMAT_UNKNOWN;
			descView.Texture2D.MipSlice = 0;

			hr = graphics->device->CreateUnorderedAccessView(texture, &descView, &unorderedView);
			if (FAILED(hr)) throw;
		}

		if (bindFlags & D3D11_BIND_RENDER_TARGET)
		{
			D3D11_RENDER_TARGET_VIEW_DESC descView = {};
			descView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			descView.Format = desc.Format;
			descView.Texture2D.MipSlice = 0;
			hr = graphics->device->CreateRenderTargetView(texture, &descView, &renderTargetView);
			if (FAILED(hr)) throw;
		}
	}
}

void Texture::SetTexture(int width, int height, int channels, void* data, DXGI_FORMAT format, D3D11_USAGE usage, UINT bindFlags, UINT CPUAccessFlags)
{
	this->~Texture();

	this->width = width;
	this->height = height;
	this->channels = channels;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = usage;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = CPUAccessFlags;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;
	sd.SysMemPitch = width * channels;
	HRESULT hr = graphics->device->CreateTexture2D(&desc, &sd, &texture);
	if (FAILED(hr)) throw;


	if (usage != D3D11_USAGE_STAGING)
	{

		if (bindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
			resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			resourceDesc.Format = desc.Format;
			resourceDesc.Texture2D.MostDetailedMip = 0;
			resourceDesc.Texture2D.MipLevels = 1;

			hr = graphics->device->CreateShaderResourceView(texture, &resourceDesc, &shaderResource);
			if (FAILED(hr)) throw;
		}

		if (bindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC descView = {};
			descView.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			descView.Format = DXGI_FORMAT_UNKNOWN;
			descView.Texture2D.MipSlice = 0;

			hr = graphics->device->CreateUnorderedAccessView(texture, &descView, &unorderedView);
			if (FAILED(hr)) throw;
		}

		if (bindFlags & D3D11_BIND_RENDER_TARGET)
		{
			D3D11_RENDER_TARGET_VIEW_DESC descView = {};
			descView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			descView.Format = desc.Format;
			descView.Texture2D.MipSlice = 0;
			hr = graphics->device->CreateRenderTargetView(texture, &descView, &renderTargetView);
			if (FAILED(hr)) throw;
		}
	}
}

void Texture::Bind(UINT slot)
{
	graphics->context->PSSetShaderResources(slot, 1, &shaderResource);
}
