#pragma once
#include "Graphics.h"

class StructuredBuffer
{
public:

	Graphics* graphics;

	ID3D11Buffer* buffer;
	ID3D11ShaderResourceView* shaderResource;

	StructuredBuffer(Graphics* graphics, UINT stride, UINT dataSize);
	StructuredBuffer(Graphics* graphics, void* data, UINT stride, UINT dataSize);
	~StructuredBuffer();

	void UploadData(void* data, UINT sizeData);

	void Bind(UINT slot);
	void BindForCompute(UINT slot);
};

