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
	ID3D11ShaderResourceView* shaderResource = nullptr;
	ID3D11UnorderedAccessView* unorderedView = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;


	Texture(Graphics* graphics, int width, int height, int channels, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT bindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);

	Texture(Graphics* graphics, int width, int height, int channels, void* data, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT bindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);
	~Texture();

	void SetTexture(int width, int height, int channels, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT bindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);

	void SetTexture(int width, int height, int channels, void* data, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT bindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);
	void Bind(UINT slot);
};

