#pragma once
#include "d3d11.h"
#include "d3dcompiler.h"

#include "..//Graphics.h"

class Shader
{
public:

	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* layout;

	Graphics* graphics;

	Shader(const wchar_t* pathVertex, const wchar_t* pathPixel, D3D11_INPUT_ELEMENT_DESC* desc, int countDesc, Graphics* graphics);
	~Shader();

	void SetShader();
};

