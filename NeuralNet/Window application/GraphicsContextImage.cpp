#include "GraphicsContextImage.h"

GraphicsContextImage::GraphicsContextImage(WindowInterfaceForNeuralNet* windowInterface)
{
	Vertex vertices[] =
	{
		{-1.0f, 1.0f, 0.0f},
		{ 1.0f, 1.0f, 0.0f},
		{ 1.0f,-1.0f, 0.0f},

		{ 1.0f,-1.0f, 0.0f},
		{-1.0f,-1.0f, 0.0f},
		{-1.0f, 1.0f, 0.0f},
	};

	this->windowInterface = windowInterface;

	//Создания класса для графики
	graphics = new Graphics(windowInterface->context);
	registerShaders = new RegisterShaders(graphics);
	vertexBuffer = new VertexBuffer(graphics, vertices, sizeof(Vertex) * 6, sizeof(Vertex));

	//Объявление текстуры и инициализация сэмплера
	sampler = new Sampler(graphics);

	//Создание константного буффера
	constantBuffer = new ConstantBuffer(graphics, sizeof(float) * 12);

	windowInterface->button->SetGraphics(graphics);
}

void GraphicsContextImage::Draw()
{
	Brush::Color* colorRGBBackground = &windowInterface->brushIdentity->colorRGB;
	FLOAT backgroundColor[4] = { colorRGBBackground->r, colorRGBBackground->g, colorRGBBackground->b, 1 };
	graphics->Clear(backgroundColor);

	constantBuffer->SetBuffer(0);

	if (windowInterface->button->texture != nullptr)
	{
		RECT rect;
		GetClientRect(windowInterface->context->hwnd, &rect);
		float size[4] = { windowInterface->button->texture->width, windowInterface->button->texture->height, rect.right, rect.bottom };
		bool isHaveTexture = true;

		int offset = 0;
		constantBuffer->Map();
		offset = constantBuffer->CopyMem(size, sizeof(float) * 4, offset);
		offset = constantBuffer->CopyMem(&windowInterface->brushIdentity->colorRGB, sizeof(float) * 4, offset);
		offset = constantBuffer->CopyMem(&isHaveTexture, sizeof(bool) * 1, offset);
		constantBuffer->UnMap();

		windowInterface->button->texture->Bind(0);
		sampler->Bind(0);

		graphics->Draw(registerShaders->shaderImage, vertexBuffer);
	}

	graphics->Display();
}
