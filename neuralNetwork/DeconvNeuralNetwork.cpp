#include "pch.h"
#include "DeconvNeuralNetwork.h"

DeconvNeuralNetwork::DeconvNeuralNetwork(DevconvNeuralNetDesc netDesc)
{
	if (netDesc.layersCount > 0)
		layersCount = netDesc.layersCount;
	else throw;

	//this->matricesCount = new int[layersCount];
	//memcpy(this->matricesCount, matricesCount, layersCount * sizeof(layersCount));

	if (netDesc.branching)
	{
		branching = new int[layersCount];
		memcpy(branching, netDesc.branching, layersCount * sizeof(int));
	}
	else throw;

	unpoolingLayers = new int[layersCount];
	if (netDesc.unpoolingLayers)
	{
		memcpy(unpoolingLayers, netDesc.unpoolingLayers, layersCount * sizeof(int));
	}
	else
	{
		std::cout << "unpoolingLayers set as default (0, 1, 0, 1, ...)" << std::endl;
		for (int i = 0; i < layersCount; i++)
			unpoolingLayers[i] = (i + 1) % 2; // Need to debug
	}

	if (netDesc.defaultKernelSize > 0)
		defaultKernelSize = netDesc.defaultKernelSize;
	else defaultKernelSize = 3;

	kernelSize = new int[layersCount];
	if (netDesc.kernelSize)
	{
		memcpy(kernelSize, netDesc.kernelSize, layersCount * sizeof(int));
	}
	else
	{
		std::cout << "kernelSize set as default " << "(" << defaultKernelSize << ", " << defaultKernelSize << ", " << defaultKernelSize << ", ...)" << std::endl;
		for (int i = 0; i < layersCount; i++)
			kernelSize[i] = defaultKernelSize;
	}

	kernelStride = new int[layersCount];
	if (netDesc.kernelStride)
	{
		memcpy(kernelStride, &netDesc.kernelStride, layersCount * sizeof(int));
	}
	else
	{
		std::cout << "kernelStride set as default (1, 1, 1, ...)" << std::endl;
		for (int i = 0; i < layersCount; i++)
			kernelStride[i] = 1;
	}

	defaultKernelOrigin = netDesc.defaultKernelOrigin;

	kernelOrigin = new int2[layersCount];
	if (netDesc.kernelOrigin)
	{
		memcpy(kernelOrigin, netDesc.kernelOrigin, layersCount * sizeof(int2));
	}
	else
	{
		for (int i = 0; i < layersCount; i++)
			kernelOrigin[i] = defaultKernelOrigin;
	}

	unpoolingSize = new int2[layersCount];
	if (netDesc.unpoolingSize)
		memcpy(unpoolingSize, netDesc.unpoolingLayers, layersCount * sizeof(int2));
	else
	{
		std::cout << "unpoolingSize set as default (kernelSize in the layer)" << std::endl;
		for (int l = 0; l < layersCount; l++)
			unpoolingSize[l] = int2{kernelSize[l], kernelSize[l]};
	}

	if (netDesc.intputMatricesSize.x > 0 && netDesc.intputMatricesSize.y > 0)
		intputMatricesSize = netDesc.intputMatricesSize;
	else throw;

	matrices = new NetMatrix * *[layersCount];
	matricesCount = new int[layersCount];
	matricesCount[0] = netDesc.inputMatricesCount;
	if (netDesc.inputMatricesCount > 0)
	{
		for (int l = 0; l < layersCount; l++)
		{
			if (l == 0)
			{
				matrices[l] = new NetMatrix * [netDesc.inputMatricesCount];
				for (int m = 0; m < netDesc.inputMatricesCount; m++)
					matrices[l][m] = new NetMatrix(intputMatricesSize.x, intputMatricesSize.y, kernelSize[l], kernelStride[l],
						kernelOrigin[l].x, kernelOrigin[l].y);
			}
			else
			{
				int2 matrixSize{};
				if (unpoolingLayers[l] <= 0)
				{
					if ((matricesCount[l - 1] % branching[l]) != 0)
						throw;
					matricesCount[l] = matricesCount[l - 1] / branching[l];
					matrixSize = int2{ matrices[l - 1][0]->matrixSizeX, matrices[l - 1][0]->matrixSizeY };
				}
				else
				{
					matricesCount[l] = matricesCount[l - 1];
					matrixSize = int2{ matrices[l - 1][0]->matrixSizeX * unpoolingSize[l].x, matrices[l - 1][0]->matrixSizeY * unpoolingSize[l].y };
				}

				matrices[l] = new NetMatrix * [matricesCount[l]];

				for (int m = 0; m < matricesCount[l]; m++)
					matrices[l][m] = new NetMatrix(matrixSize.x, matrixSize.y, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
			}
		}
	}
	else throw;
}

void DeconvNeuralNetwork::forwardPropagation(void* input, const int inputDataSize)
{
	int inputMatricesDataCount = ((intputMatricesSize.x * intputMatricesSize.y) * matricesCount[0]) * sizeof(matrices[0][0]->matrix[0][0]);
	if (inputMatricesDataCount != inputDataSize) throw;

	for (int m = 0; m < matricesCount[0]; m++)
	{
		for(int y = 0; y < matrices[0][m]->matrixSizeY; y++)
			for (int x = 0; x < matrices[0][m]->matrixSizeX; x++)
			{
				int id = x + y * matrices[0][m]->matrixSizeX + m * matrices[0][m]->matrixSizeX * matrices[0][m]->matrixSizeY;
				matrices[0][m]->matrix[x][y] = ((double*)input)[id];
			}
	}

	for(int l = 0; l < layersCount - 1; l++)
		for (int m = 0; m < matricesCount[l]; m++)
		{
			if (unpoolingLayers[l + 1] > 0)
				matrices[l][m]->unpool(matrices[l + 1][m], unpoolingSize[l + 1].x, unpoolingSize[l + 1].y);
			else
			{
				int nextLayerMatrixID = m / branching[l + 1];
				matrices[l][m]->deconvolute(matrices[l + 1][nextLayerMatrixID]);
			}
		}
}

void DeconvNeuralNetwork::setAllWeightsRandom(int seed, int leftEdge, int rightEdge, int accuracy)
{
	for(int l = 0; l < layersCount; l++)
		for (int m = 0; m < matricesCount[l]; m++)
			matrices[l][m]->setRandomWeights(seed, leftEdge, rightEdge, accuracy);
}

DeconvNeuralNetwork::~DeconvNeuralNetwork()
{
	delete[layersCount] branching;
	delete[layersCount] unpoolingLayers;
	delete[layersCount] kernelSize;
	delete[layersCount] kernelStride;
	delete[layersCount] kernelOrigin;
	delete[layersCount] unpoolingSize;

	// Deleting matrices
	for (int l = 0; l < layersCount; l++)
		for (int m = 0; m < matricesCount[l]; m++)
			delete matrices[l][m];
	for (int l = 0; l < layersCount; l++)
		delete[matricesCount[l]] matrices[l];
	delete[layersCount] matrices;

	delete[layersCount] matricesCount;
}
