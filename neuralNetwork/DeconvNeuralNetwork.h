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
	
	struct PartialDevconvNeuralNetDesc
	{
		int* branching;
		int defaultKernelSize;
		int* kernelSize;
		int defaultKernelStride;
		int* kernelStride;
		int2 defaultKernelOrigin;
		int2* kernelOrigin;
		int2 defaultUnpoolingSize;
		int2* unpoolingSize;
	};
	int layersCount;
	int* branching;
	int* kernelSize;
	int defaultKernelStride;
	int* kernelStride;
	int2 defaultKernelOrigin;
	int2* kernelOrigin;
	int2* unpoolingSize;
	int2 defaultUnpoolingSize;
	int* matricesCount;
	int2 outputImageSize;
	NetMatrix*** matrices;
	NetMatrix*** errorMatrices;

	DECL DeconvNeuralNetwork(int2 outputImageSize, int layersCount, PartialDevconvNeuralNetDesc netDesc);
	DECL void forwardPropagation(void* input, const int inputDataSize);
	DECL void calculateErrors(double** output);
	DECL void correctWeights(double k, double a);
	DECL void backPropagation(double** output, const int outputDataSize, double k, double a);
	DECL void setAllWeightsRandom(int seed, int leftEdge, int rightEdge, int accuracy);
	DECL void getParentMatrix(int childLayer, int childMatrixID, int* parentLayer, int* parentMatrixID);
	DECL NetMatrix* getParentMatrix(int childLayer, int childMatrixID);
	DECL NetMatrix* getParentErrorMatrix(int childLayer, int childMatrixID);
	DECL double getError(double** output, const int outputDataSize);
	DECL ~DeconvNeuralNetwork();
private:
	int defaultKernelSize;
};

