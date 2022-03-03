#pragma once
#include "Decl.h"
#include "NetMatrix.h"
#include <iostream>

class DeconvNeuralNetwork
{
public:
	struct int2
	{
		int x;
		int y;
	};
	struct DevconvNeuralNetDesc
	{
		int layersCount;
		int* branching;
		int* unpoolingLayers;
		int defaultKernelSize;
		int* kernelSize;
		int* kernelStride;
		int2 defaultKernelOrigin;
		int2* kernelOrigin;
		int2* unpoolingSize;
		int2 intputMatricesSize;
		int inputMatricesCount;
	};
	int layersCount; //
	int* branching; //
	int* unpoolingLayers; //
	int* kernelSize;
	int* kernelStride;
	int2 defaultKernelOrigin;
	int2* kernelOrigin;
	int2* unpoolingSize; //
	int2 intputMatricesSize; //
	int* matricesCount; //

	NetMatrix*** matrices;


	DECL DeconvNeuralNetwork(DevconvNeuralNetDesc netDesc);
	DECL ~DeconvNeuralNetwork();
private:
	int defaultKernelSize;
};

