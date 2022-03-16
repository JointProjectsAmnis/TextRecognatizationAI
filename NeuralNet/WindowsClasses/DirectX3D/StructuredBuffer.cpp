#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(Graphics* graphics, UINT stride, UINT dataSize)
{
	this->graphics = graphics;

	D3D11_BUFFER_DESC sbDesc;
	//sbDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sbDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS; // Может быть ошибка из-за D3D11_BIND_UNORDERED_ACCESS 
	sbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sbDesc.Usage = D3D11_USAGE_DYNAMIC;
	sbDesc.StructureByteStride = stride;
	sbDesc.ByteWidth = dataSize;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	HRESULT hr = graphics->device->CreateBuffer(&sbDesc, NULL, &buffer);
	if (FAILED(hr)) throw;

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_Desc = {};
	srv_Desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv_Desc.Buffer.ElementWidth = stride;
	srv_Desc.Buffer.NumElements = dataSize / stride;
	srv_Desc.Buffer.FirstElement = 0;
	srv_Desc.Buffer.ElementOffset = 0;

	hr = graphics->device->CreateShaderResourceView(buffer, &srv_Desc, &shaderResource);
	if (FAILED(hr)) throw;
}

StructuredBuffer::StructuredBuffer(Graphics* graphics, void* data, UINT stride, UINT dataSize)
{
	this->graphics = graphics;

	D3D11_BUFFER_DESC sbDesc;
	sbDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sbDesc.Usage = D3D11_USAGE_DYNAMIC;
	sbDesc.StructureByteStride = stride;
	sbDesc.ByteWidth = dataSize;
	sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = data;
	HRESULT hr = graphics->device->CreateBuffer(&sbDesc, &sd, &buffer);
	if (FAILED(hr)) throw;

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_Desc = {};
	srv_Desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_Desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv_Desc.Buffer.ElementWidth = stride;
	srv_Desc.Buffer.NumElements = dataSize / stride;
	srv_Desc.Buffer.FirstElement = 0;
	srv_Desc.Buffer.ElementOffset = 0;

	hr = graphics->device->CreateShaderResourceView(buffer, &srv_Desc, &shaderResource);
	if (FAILED(hr)) throw;
}

StructuredBuffer::~StructuredBuffer()
{
	buffer->Release();
}

void StructuredBuffer::UploadData(void* data, UINT sizeData)
{
	D3D11_MAPPED_SUBRESOURCE mappedSub = {};
	HRESULT hr = graphics->context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSub);
	if (FAILED(hr)) throw;
	memcpy(mappedSub.pData, data, sizeData);
	graphics->context->Unmap(buffer, 0);
}

void StructuredBuffer::Bind(UINT slot)
{
	graphics->context->PSSetShaderResources(slot, 1, &shaderResource);
}