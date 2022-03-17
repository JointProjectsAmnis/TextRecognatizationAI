#pragma once
#include "d3d11.h"
#include "d3dcompiler.h"

#include "..//BaseWindow.h"

class Shader;
class VertexBuffer;

class Graphics
{
public:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapchain;
	ID3D11RenderTargetView* renderTarget;

	BaseWindow* window;

	Graphics(BaseWindow* window);
	~Graphics();

	void CleanD3D();
	void UpdateViewport();

	void Clear(const FLOAT* color);
	void Draw(Shader* shader, VertexBuffer* vertexBuffer);
	void Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ); //Запустить компьютерный шейдер
	void Display();
};

