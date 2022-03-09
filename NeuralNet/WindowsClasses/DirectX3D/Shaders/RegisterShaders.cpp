#include "RegisterShaders.h"

RegisterShaders::RegisterShaders(Graphics* graphics)
{
	this->graphics = graphics;
	InitShaders();
}

RegisterShaders::~RegisterShaders()
{
	for (int x = 0; x < shader.size(); x++)
		delete shader[x];
}

void RegisterShaders::AddShader(Shader* shader)
{
	RegisterShaders::shader.push_back(shader);
}

void RegisterShaders::InitShaders()
{
	D3D11_INPUT_ELEMENT_DESC iedDefault[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	shaderDefault = new Shader(L"WindowsClasses\\DirectX3D\\Shaders\\ShaderDefault\\ShaderVertexDefault.hlsl", L"WindowsClasses\\DirectX3D\\Shaders\\ShaderDefault\\ShaderPixelDefault.hlsl", iedDefault, 1, graphics);
	shader.push_back(shaderDefault);

	D3D11_INPUT_ELEMENT_DESC iedImage[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	shaderImage = new Shader(L"WindowsClasses\\DirectX3D\\Shaders\\ShaderImage\\ShaderVertexImage.hlsl", L"WindowsClasses\\DirectX3D\\Shaders\\ShaderImage\\ShaderPixelImage.hlsl", iedImage, 1, graphics);
	shader.push_back(shaderImage);

	D3D11_INPUT_ELEMENT_DESC iedTriangle[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	shaderTriangle = new Shader(L"WindowsClasses\\DirectX3D\\Shaders\\ShaderTriangle\\ShaderVertexTriangle.hlsl", L"WindowsClasses\\DirectX3D\\Shaders\\ShaderTriangle\\ShaderPixelTriangle.hlsl", iedTriangle, 2, graphics);
	shader.push_back(shaderTriangle);
}
