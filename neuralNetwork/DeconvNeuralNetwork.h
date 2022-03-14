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
		int2 defaultUnpoolingSize;
		int2 intputMatricesSize;
		int inputMatricesCount;
		int2 outputImageSize;
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
	int layersCount; //
	int* branching; //
	int* unpoolingLayers; //
	int* kernelSize;
	int defaultKernelStride;
	int* kernelStride;
	int2 defaultKernelOrigin;
	int2* kernelOrigin;
	int2* unpoolingSize; //
	int2 intputMatricesSize; //
	int2 defaultUnpoolingSize;
	int* matricesCount; //
	int2 outputImageSize;
	NetMatrix*** matrices;
	NetMatrix*** errorMatrices;

	DECL DeconvNeuralNetwork(DevconvNeuralNetDesc netDesc);
	DECL DeconvNeuralNetwork(int2 outputImageSize, int layersCount, PartialDevconvNeuralNetDesc netDesc);
	DECL void forwardPropagation(void* input, const int inputDataSize);
	DECL void forwardPropagationWithoutUnPoolingLayers(void* input, const int inputDataSize);
	DECL void calculateErrors(double** output);
	DECL void calculateErrorsWithoutUnPoolingLayers(double** output);
	DECL void correctWeights(double k, double a);
	DECL void correctWeightsWithoutUnPoolingLayers(double k, double a);
	DECL void backPropagation(double** output, const int outputDataSize, double k, double a);
	DECL void backPropagationWithoutUnPoolingLayers(double** output, const int outputDataSize, double k, double a);
	DECL void setAllWeightsRandom(int seed, int leftEdge, int rightEdge, int accuracy);
	DECL void getParentMatrix(int childLayer, int childMatrixID, int* parentLayer, int* parentMatrixID);
	DECL void getParentMatrixWithoutUnPoolingLayers(int childLayer, int childMatrixID, int* parentLayer, int* parentMatrixID);
	DECL NetMatrix* getParentMatrix(int childLayer, int childMatrixID);
	DECL NetMatrix* getParentMatrixWithoutUnPoolingLayers(int childLayer, int childMatrixID);
	DECL NetMatrix* getParentErrorMatrix(int childLayer, int childMatrixID);
	DECL NetMatrix* getParentErrorMatrixWithoutUnPoolingLayers(int childLayer, int childMatrixID);
	DECL double getError(double** output, const int outputDataSize);
	DECL ~DeconvNeuralNetwork();
private:
	int defaultKernelSize;
};

