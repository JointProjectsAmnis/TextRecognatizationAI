#include "ShaderCompute.h"

ShaderCompute::ShaderCompute(Graphics* graphics, const wchar_t* path)
{
	this->graphics = graphics;

	HRESULT hr = D3DCompileFromFile(path, 0, 0, "main", "cs_5_0", 0, 0, &shaderBlob, nullptr);
	if (FAILED(hr)) throw;

	hr = graphics->device->CreateComputeShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &computerShader);
	if (FAILED(hr)) throw;
}

ShaderCompute::~ShaderCompute()
{
}

void ShaderCompute::BindShader()
{
	graphics->context->CSSetShader(computerShader, NULL, 0);
}
