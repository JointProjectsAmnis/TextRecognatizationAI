#pragma once
#include "Window application/WindowInterfaceForNeuralNet.h"
#include "Window application/GraphicsContextImage.h"
#include "Window application/Content.h"

#include "WindowsClasses/DirectX3D/Shaders/ShaderCompute.h"
#include "WindowsClasses/DirectX3D/UnorderedBuffer.h"

class GPUNetMatrix
{
public:
	int matrixSizeX;
	int matrixSizeY;

	int kernelSize;
	int kernelStride;
	int kernelOriginX;
	int kernelOriginY;

	Texture* matrix;
	Texture* kernel;
	Texture* oldDeltaKernel;

	GPUNetMatrix(Graphics* graphics, int matrixSizeX, int matrixSizeY, int kernelSize = -1, int kernelStride = -1, int kernelOriginX = -1, int kernelOriginY = -1);
	~GPUNetMatrix();
	void clear(double value);
	double sigmoid(double x);
	void setRandomWeights(int seed, int module, double devider, double offset);
	void setRandomWeights(int seed, int leftEdge, int rightEdge, int accuracy);
	void fill(double** data);
private:
	DXGI_FORMAT matricesFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	int matricesChannelsCount = 1;
};

