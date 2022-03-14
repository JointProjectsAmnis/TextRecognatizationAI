#include "pch.h"
#include "ConvNeuralNetwork.h"


ConvNeuralNetwork::ConvNeuralNetwork(int2 outputImageSize, int layersCount, ConvNeuralNetDesc netDesc)
{
	if (outputImageSize.x <= 0 || outputImageSize.y <= 0) throw;
	this->outputImageSize = outputImageSize;

	if (layersCount <= 0) throw;
	this->layersCount = layersCount;

	if (!netDesc.branching)	throw;
	this->branching = new int[layersCount];
	memcpy(this->branching, netDesc.branching, layersCount * sizeof(int));


	kernelSize = new int[layersCount];
	if (netDesc.kernelSize)
		memcpy(kernelSize, &netDesc.kernelSize, layersCount * sizeof(int));
	else
	{
		if (netDesc.defaultKernelSize > 0)
		{
			defaultKernelSize = netDesc.defaultKernelSize;
			for (int l = 0; l < layersCount; l++)
				kernelSize[l] = defaultKernelSize;
		}
		else
			throw;
	}

	kernelStride = new int[layersCount];
	if (netDesc.kernelStride)
		memcpy(kernelStride, &netDesc.kernelStride, layersCount * sizeof(int));
	else
	{

		if (netDesc.defaultKernelStride > 0)
		{
			defaultKernelStride = netDesc.defaultKernelStride;
			for (int i = 0; i < layersCount; i++)
				kernelStride[i] = defaultKernelStride;
		}
		else
		{
			for (int i = 0; i < layersCount; i++)
				kernelStride[i] = 1;
		}
	}

	kernelOrigin = new int2[layersCount];
	if (netDesc.kernelOrigin)
		memcpy(kernelOrigin, netDesc.kernelOrigin, layersCount * sizeof(int2));
	else
	{
		if (netDesc.defaultKernelOrigin.x > 0 && netDesc.defaultKernelOrigin.y > 0)
		{
			defaultKernelOrigin = netDesc.defaultKernelOrigin;
			for (int i = 0; i < layersCount; i++)
				kernelOrigin[i] = defaultKernelOrigin;
		}
		else
			throw;
	}

	poolingSize = new int2[layersCount];
	if (netDesc.poolingSize)
		memcpy(poolingSize, netDesc.poolingSize, layersCount * sizeof(int2));
	else
	{
		if (netDesc.defaultPoolingSize.x > 0 && netDesc.defaultPoolingSize.y > 0)
		{
			defaultPoolingSize = netDesc.defaultPoolingSize;
			for (int i = 0; i < layersCount; i++)
				poolingSize[i] = defaultPoolingSize;
		}
		else
			throw;
	}

	matrices = new NetMatrix * *[layersCount];
	matricesCount = new int[layersCount];
	for (int l = 0; l < layersCount; l++)
	{
		if (l == 0)
		{
			matricesCount[l] = 1;
			matrices[l] = new NetMatrix * [matricesCount[l]];
			matrices[l][0] = new NetMatrix(outputImageSize.x, outputImageSize.y, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
		}
		else
		{
			if (branching[l - 1] > 0)
			{
				matricesCount[l] = branching[l - 1] * matricesCount[l - 1];
				matrices[l] = new NetMatrix * [matricesCount[l]];
				int matrixSizeX = matrices[l - 1][0]->matrixSizeX;
				int matrixSizeY = matrices[l - 1][0]->matrixSizeY;
				for (int m = 0; m < matricesCount[l]; m++)
					matrices[l][m] = new NetMatrix(matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
			}
			else
			{
				matricesCount[l] = matricesCount[l - 1];
				matrices[l] = new NetMatrix * [matricesCount[l]];

				int matrixSizeX = ceil(matrices[l - 1][0]->matrixSizeX / (double)poolingSize[l].x);
				int matrixSizeY = ceil(matrices[l - 1][0]->matrixSizeY / (double)poolingSize[l].y);
				for (int m = 0; m < matricesCount[l]; m++)
					matrices[l][m] = new NetMatrix(matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
			}
		}
	}

	errorMatrices = new NetMatrix * *[layersCount];

	for (int l = 0; l < layersCount; l++)
		errorMatrices[l] = new NetMatrix * [matricesCount[l]];

	for (int l = 0; l < layersCount; l++)
	{
		int matrixSizeX = matrices[l][0]->matrixSizeX;
		int matrixSizeY = matrices[l][0]->matrixSizeY;

		for (int m = 0; m < matricesCount[l]; m++)
			errorMatrices[l][m] = new NetMatrix(matrixSizeX, matrixSizeY);
	}

}

ConvNeuralNetwork::~ConvNeuralNetwork()
{
	if (branching) delete[layersCount] branching;
	if (kernelSize) delete[layersCount] kernelSize;
	if (kernelStride) delete[layersCount] kernelStride;
	if (kernelOrigin) delete[layersCount] kernelOrigin;
	if (poolingSize) delete[layersCount] poolingSize;

	// Deleting matrices

	for (int l = 0; l < layersCount; l++)
		for (int m = 0; m < matricesCount[l]; m++)
			if (matrices[l][m]) delete matrices[l][m];
	for (int l = 0; l < layersCount; l++)
		if (matrices[l]) delete[matricesCount[l]] matrices[l];
	if (matrices) delete[layersCount] matrices;

	// Deleting errorMatrices

	for (int l = 0; l < layersCount; l++)
		for (int m = 0; m < matricesCount[l]; m++)
			if (errorMatrices[l][m]) delete errorMatrices[l][m];
	for (int l = 0; l < layersCount; l++)
		if (errorMatrices[l]) delete[matricesCount[l]] errorMatrices[l];
	if (errorMatrices) delete[layersCount] errorMatrices;

	if (matricesCount) delete[layersCount] matricesCount;
}

void ConvNeuralNetwork::forwardPropagation(double** inputData, const int inputDataSize)
{
	int inputMatrixDataCount = (matrices[0][0]->matrixSizeX * matrices[0][0]->matrixSizeY) * sizeof(matrices[0][0]->matrix[0][0]);
	if (inputMatrixDataCount != inputDataSize) throw;

	matrices[0][0]->fill(inputData);
	for (int l = 1; l < layersCount; l++)
	{
		for (int m = 0; m < matricesCount[layersCount]; m++)
		{

		}
	}

}
