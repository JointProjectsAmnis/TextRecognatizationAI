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
	DECL void calculateErrors(double* output);
	DECL void backPropagation(double** output, const int outputDataSize, double k, double a);
	DECL void getParentMatrix(int childLayer, int childMatrixID, int* parentLayer, int* parentMatrixID);
	DECL NetMatrix* getParentMatrix(int childLayer, int childMatrixID);
	DECL NetMatrix* getParentErrorMatrix(int childLayer, int childMatrixID);
	DECL int getChildMatricesCount(int l, int m);
	DECL void getChildMatrix(int l, int m, int childID, int* childLayer, int* childMatrixID);
	DECL NetMatrix* getChildMatrix(int l, int m, int childID);
	DECL NetMatrix* getChildErrorMatrix(int l, int m, int childID);
	DECL void setAllWeightsRandom(int seed, int leftEdge, int rightEdge, int accuracy);
};

