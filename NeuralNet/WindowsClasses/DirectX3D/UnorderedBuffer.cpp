#include "UnorderedBuffer.h"

UnorderedBuffer::UnorderedBuffer(Graphics* graphics, UINT sizeData, UINT stride, void* data)
{
	this->graphics = graphics;

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.ByteWidth = sizeData;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;

	HRESULT hr = graphics->device->CreateBuffer(&desc, &sd, &buffer);
	if (FAILED(hr)) throw; //Возможно ошибка в том, что stride не кратен 4
	

	D3D11_UNORDERED_ACCESS_VIEW_DESC descView = {};
	descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descView.Buffer.FirstElement = 0;
	descView.Format = DXGI_FORMAT_UNKNOWN;
	descView.Buffer.NumElements = sizeData / stride;

	hr = graphics->device->CreateUnorderedAccessView(buffer, &descView, &unorderedView);
	if (FAILED(hr)) throw;
}

void UnorderedBuffer::Map(D3D11_MAP mapType)
{
	D3D11_MAPPED_SUBRESOURCE mappedSub = {};
	HRESULT hr = graphics->context->Map(buffer, 0, mapType, 0, &mappedSub);
	if (FAILED(hr)) throw;
	mappedAddress = (char*)mappedSub.pData;
}

int UnorderedBuffer::CopyMem(void* data, size_t sizeData, size_t offset)
{
	memcpy(mappedAddress + offset, data, sizeData);
	return offset + sizeData;
}

void UnorderedBuffer::UnMap()
{
	graphics->context->Unmap(buffer, 0);
}

void UnorderedBuffer::Bind(UINT slot)
{
	graphics->context->CSSetUnorderedAccessViews(slot, 1, &unorderedView, NULL);
}
