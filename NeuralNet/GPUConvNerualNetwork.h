#pragma once
#include "Window application/WindowInterfaceForNeuralNet.h"
#include "Window application/GraphicsContextImage.h"
#include "Window application/Content.h"

#include "WindowsClasses/DirectX3D/Shaders/ShaderCompute.h"
#include "WindowsClasses/DirectX3D/UnorderedBuffer.h"
#include "GPUNetMatrix.h"
#include "mymath.h"

class GPUConvNerualNetwork
{
public: 
	struct GPUConvNeuralNetDesc
	{
		int* branching;
		int defaultKernelSize;
		int* kernelSize;
		int defaultKernelStride;
		int* kernelStride;
		mint2 defaultKernelOrigin;
		mint2* kernelOrigin;
		mint2 defaultPoolingSize;
		mint2* poolingSize;
	};
	int layersCount;
	int* branching;
	int defaultKernelSize;
	int* kernelSize;
	int defaultKernelStride;
	int* kernelStride;
	mint2 defaultKernelOrigin;
	mint2* kernelOrigin;
	mint2* poolingSize;
	mint2 defaultPoolingSize;
	int* matricesCount;
	mint2 inputMatrixSize;
	mint2* matricesSize;

	GPUNetMatrix*** matrices;
	GPUNetMatrix*** errorMatrices;

	//Texture*** matrices;
	//Texture*** kernels;



	GPUConvNerualNetwork(Graphics* graphics, mint2 inputMatrixSize, int layersCount, GPUConvNeuralNetDesc netDesc);
	~GPUConvNerualNetwork();

	void forwardPropagation(Graphics* graphics, double** inputData, const int inputDataSize);
	void backPropagation();

private:
	ID3D11UnorderedAccessView*** matricesTextures;
	ShaderCompute* shaderForwardProgation;
};

