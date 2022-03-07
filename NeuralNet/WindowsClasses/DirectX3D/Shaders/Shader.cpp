#include "Shader.h"

Shader::Shader(const wchar_t* pathVertex, const wchar_t* pathPixel, D3D11_INPUT_ELEMENT_DESC* desc, int countDesc, Graphics* graphics)
{
	this->graphics = graphics;

	HRESULT hr = D3DCompileFromFile(pathVertex, 0, 0, "main", "vs_5_0", 0, 0, &vertexShaderBlob, nullptr);
	if (FAILED(hr)) throw;

	hr = D3DCompileFromFile(pathPixel,  0, 0, "main", "ps_5_0", 0, 0, &pixelShaderBlob, nullptr);
	if (FAILED(hr)) throw;



	hr = graphics->device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, &vertexShader);
	if (FAILED(hr)) throw;

	hr = graphics->device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, &pixelShader);
	if (FAILED(hr)) throw;


	hr = graphics->device->CreateInputLayout(desc, countDesc, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &layout);
	if (FAILED(hr)) throw;
}

Shader::~Shader()
{
	vertexShaderBlob->Release();
	pixelShaderBlob->Release();

	vertexShader->Release();
	pixelShader->Release();

	layout->Release();
}

void Shader::SetShader()
{
	graphics->context->PSSetShader(pixelShader, NULL, NULL);
	graphics->context->VSSetShader(vertexShader, NULL, NULL);
	graphics->context->IASetInputLayout(layout);
}
