#pragma once

//DirectX
#include "../WindowsClasses/DirectX3D/Graphics.h"
#include "../WindowsClasses/DirectX3D/Shaders/RegisterShaders.h"
#include "../WindowsClasses/DirectX3D/Sampler.h"
#include "../WindowsClasses/DirectX3D/VertexBuffer.h"
#include "../WindowsClasses/DirectX3D/ConstantBuffer.h"

//Window application
#include "WindowInterfaceForNeuralNet.h"

class GraphicsContextImage
{
public:

	struct Vertex
	{
		FLOAT x, y, z;
		FLOAT color[4];
	};

	//�������
	Graphics* graphics;

	//�������
	VertexBuffer* vertexBuffer;
	ConstantBuffer* constantBuffer;

	//�������� � �������
	RegisterShaders* registerShaders;
	Sampler* sampler;

	//������� ���������
	WindowInterfaceForNeuralNet* windowInterface;

	GraphicsContextImage(WindowInterfaceForNeuralNet* windowInterface);

	void Draw();
};

