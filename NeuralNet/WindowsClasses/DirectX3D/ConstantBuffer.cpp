#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(Graphics* graphics, UINT sizeData)
{
	this->graphics = graphics;

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0u;
	desc.ByteWidth = sizeData;
	desc.StructureByteStride = 0u;

	HRESULT hr = graphics->device->CreateBuffer(&desc, NULL, &buffer);
	if (FAILED(hr)) throw;
}

ConstantBuffer::ConstantBuffer(Graphics* graphics, void* data, UINT sizeData)
{
	this->graphics = graphics;

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0u;
	desc.ByteWidth = sizeData;
	desc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;

	HRESULT hr = graphics->device->CreateBuffer(&desc, &sd, &buffer);
	if (FAILED(hr)) throw;
}

ConstantBuffer::~ConstantBuffer()
{
	buffer->Release();
}

void ConstantBuffer::SetBuffer(UINT slot)
{
	graphics->context->PSSetConstantBuffers(0, 1, &buffer);
}

void ConstantBuffer::Map(D3D11_MAP mapType)
{
	D3D11_MAPPED_SUBRESOURCE mappedSub = {};
	graphics->context->Map(buffer, 0, mapType, 0, &mappedSub);
	mappedAddress = (char*)mappedSub.pData;
}

size_t ConstantBuffer::CopyMem(const void* data, size_t dataSize, size_t offset)
{
	memcpy(mappedAddress + offset, data, dataSize);
	return offset + dataSize;
}

void ConstantBuffer::UnMap()
{
	graphics->context->Unmap(buffer, 0);
}

void ConstantBuffer::ToSetData(void* data, size_t dataSize)
{
	Map();
	CopyMem(data, dataSize, 0);
	UnMap();
}
