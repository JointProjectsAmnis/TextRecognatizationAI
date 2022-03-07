#pragma once
#include "Graphics.h"

class VertexBuffer
{
public:

	ID3D11Buffer* buffer;
	UINT vertexSize;
	UINT vertexCount;

	Graphics* graphics;

	VertexBuffer(Graphics* graphics, void* data, size_t dataSize, UINT vertexSize);
	
	void SetVertexBuffer();
};

