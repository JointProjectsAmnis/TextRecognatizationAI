#include "Graphics.h"
#include "VertexBuffer.h"
#include "Shaders/Shader.h"

Graphics::Graphics(BaseWindow* window)
{
	this->window = window;

	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.Windowed = true;
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.OutputWindow = window->hwnd;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
	NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&desc,
		&swapchain,
		&device,
		NULL,
		&context);

	if (FAILED(hr)) throw;

	ID3D11Texture2D* backBuffer;
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(hr)) throw;


	hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTarget);
	if (FAILED(hr)) throw;

	context->OMSetRenderTargets(1, &renderTarget, NULL);

	UpdateViewport();
}

Graphics::~Graphics()
{
	CleanD3D();
}

void Graphics::CleanD3D()
{
	device->Release();
	context->Release();
	swapchain->Release();
	renderTarget->Release();
}

void Graphics::UpdateViewport()
{
	RECT rect;
	GetClientRect(window->hwnd, &rect);
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = rect.left;
	viewport.TopLeftY = rect.top;
	viewport.Width = rect.right;
	viewport.Height = rect.bottom;

	context->RSSetViewports(1, &viewport);
}

void Graphics::Clear(const FLOAT* color)
{
	context->ClearRenderTargetView(renderTarget, color);
}

void Graphics::Draw(Shader* shader, VertexBuffer* vertexBuffer)
{
	vertexBuffer->SetVertexBuffer();
	shader->SetShader();

	context->OMSetRenderTargets(1, &renderTarget, NULL);
	context->Draw(vertexBuffer->vertexCount, 0);
}

void Graphics::Dispatch(UINT ThreadGroupCountX,UINT ThreadGroupCountY, UINT ThreadGroupCountZ)
{
	context->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
}

void Graphics::Display()
{
	swapchain->Present(0, 0);
}
