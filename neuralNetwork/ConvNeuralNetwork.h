#pragma once
#include "Decl.h"
#include "NetMatrix.h"
#include "amnisMath.h"
#include <iostream>

class ConvNeuralNetwork
{
public:
	struct ConvNeuralNetDesc
	{
		int* branching;
		int defaultKernelSize;
		int* kernelSize;
		int defaultKernelStride;
		int* kernelStride;
		int2 defaultKernelOrigin;
		int2* kernelOrigin;
		int2 defaultPoolingSize;
		int2* poolingSize;
	};

	int layersCount;
	int* branching;
	int defaultKernelSize;
	int* kernelSize;
	int defaultKernelStride;
	int* kernelStride;
	int2 defaultKernelOrigin;
	int2* kernelOrigin;
	int2* poolingSize;
	int2 defaultPoolingSize;
	int* matricesCount;
	int2 outputImageSize;
	NetMatrix*** matrices;
	NetMatrix*** errorMatrices;

	DECL ConvNeuralNetwork(int2 outputImageSize, int layersCount, ConvNeuralNetDesc netDesc);
	DECL ~ConvNeuralNetwork();
	DECL void forwardPropagation(double** inputData, const int inputDataSize);
	DECL void getParentMatrix(int childLayer, int childMatrixID, int* parentLayer, int* parentMatrixID);
	DECL NetMatrix* getParentMatrix(int childLayer, int childMatrixID);
};

