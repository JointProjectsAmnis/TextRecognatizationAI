#pragma once

#include <windows.h>

#include "d3d11.h"
#include "d3dcompiler.h"
#include "../Graphics.h"

class ShaderCompute
{
public:

	ID3DBlob* shaderBlob;

	ID3D11ComputeShader* computerShader;

	Graphics* graphics;

	ShaderCompute(Graphics* graphics, const wchar_t* path);
	~ShaderCompute();

	void BindShader();
};

