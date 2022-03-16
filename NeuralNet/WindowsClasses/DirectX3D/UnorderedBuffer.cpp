#include "UnorderedBuffer.h"

UnorderedBuffer::UnorderedBuffer(Graphics* graphics, UINT sizeData, UINT stride, void* data)
{
	this->graphics = graphics;

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = sizeData;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;

	graphics->device->CreateBuffer(&desc, &sd, &buffer);

	

	D3D11_UNORDERED_ACCESS_VIEW_DESC descView;

}
