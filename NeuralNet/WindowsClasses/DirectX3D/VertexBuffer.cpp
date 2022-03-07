#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics* graphics, void* data, size_t dataSize, UINT vertexSize)
{
	this->vertexSize = vertexSize;
	this->graphics = graphics;
	this->vertexCount = dataSize / vertexSize;

	D3D11_BUFFER_DESC desc = {0};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = dataSize;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;

	HRESULT hr = graphics->device->CreateBuffer(&desc, &sd, &buffer);
	if (FAILED(hr)) throw;
}

void VertexBuffer::SetVertexBuffer()
{
	UINT offset = 0;
	graphics->context->IASetVertexBuffers(0, 1, &buffer, &vertexSize, &offset);
	graphics->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
