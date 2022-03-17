#pragma once

#include "Graphics.h"

class UnorderedBuffer
{
public:

	ID3D11Buffer* buffer;
	ID3D11UnorderedAccessView* unorderedView;

	Graphics* graphics;

	//stride должен быть кратен 4
	UnorderedBuffer(Graphics* graphics, UINT sizeData, UINT stride, void* data);

	void Map(D3D11_MAP mapType = D3D11_MAP_READ_WRITE);
	int CopyMem(void* data, size_t sizeData, size_t offset);
	void UnMap();

	void Bind(UINT slot);

private:
	char* mappedAddress;
};

