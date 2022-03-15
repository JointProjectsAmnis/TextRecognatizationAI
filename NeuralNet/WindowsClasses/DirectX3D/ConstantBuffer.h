#pragma once
#include "Graphics.h"

class ConstantBuffer
{
public:

	ID3D11Buffer* buffer;

	Graphics* graphics;

	ConstantBuffer(Graphics* graphics, UINT sizeData);
	ConstantBuffer(Graphics* graphics, void* data, UINT sizeData);
	~ConstantBuffer();
	
	void SetBuffer(UINT slot);

	void Map(D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD);
	size_t CopyMem(const void* data, size_t dataSize, size_t offset);
	void UnMap();

	void ToSetData(void* data, size_t dataSize);

private:
	char* mappedAddress;
};

