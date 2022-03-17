#pragma once

#include "Graphics.h"

class UnorderedBuffer
{
public:

	ID3D11Buffer* buffer;
	ID3D11UnorderedAccessView* unorderedView;

	Graphics* graphics;

	//stride должен быть кратен 4
	UnorderedBuffer(Graphics* graphics, UINT sizeData, UINT stride, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT bindFlags = D3D11_BIND_UNORDERED_ACCESS, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);

	//stride должен быть кратен 4
	UnorderedBuffer(Graphics* graphics, UINT sizeData, UINT stride, void* data, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT bindFlags = D3D11_BIND_UNORDERED_ACCESS, UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE);

	void Map(D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD);
	int WriteMem(void* data, size_t sizeData, size_t offset);
	int ReadMem(void* data, size_t sizeData, size_t offset);
	void UnMap();

	void Bind(UINT slot);

	void CopyResource(ID3D11Buffer* buffer);

private:
	char* mappedAddress;
};

