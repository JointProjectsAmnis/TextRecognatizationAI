#pragma once
#include "Decl.h"
#include "NetMatrix.h"
//#include "Data/SampleData.h"
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
	NetMatrix*** errorMatrices;

	DECL DeconvNeuralNetwork(DevconvNeuralNetDesc netDesc);
	DECL void forwardPropagation(void* input, const int inputDataSize);
	DECL void calculateErrors(double** output);
	DECL void correctWeights();
	DECL void backPropagation(double** output, const int outputDataSize);
	DECL void setAllWeightsRandom(int seed, int leftEdge, int rightEdge, int accuracy);
	DECL void getParentMatrix(int childLayer, int childMatrixID, int* parentLayer, int* parentMatrixID);
	DECL NetMatrix* getParentMatrix(int childLayer, int childMatrixID);
	DECL NetMatrix* getParentErrorMatrix(int childLayer, int childMatrixID);
	DECL ~DeconvNeuralNetwork();
private:
	int defaultKernelSize;
};

