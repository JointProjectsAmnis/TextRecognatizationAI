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
		if (branching[l - 1] > 0)
		{
			for (int m = 0; m < matricesCount[l]; m++)
			{
				NetMatrix* parentMatrix = getParentMatrix(l, m);
				parentMatrix->convolute(matrices[l][m]);
			}
		}
		else
		{
			for (int m = 0; m < matricesCount[l]; m++)
			{
				NetMatrix* parentMatrix = getParentMatrix(l, m);
				parentMatrix->pool(matrices[l][m], poolingSize[l - 1].x, poolingSize[l - 1].y);
			}
		}
	}
}

void ConvNeuralNetwork::calculateErrors(double* output)
{
	for (int m = 0; m < matricesCount[layersCount - 1]; m++)
	{
		for (int y = 0; y < errorMatrices[layersCount - 1][m]->matrixSizeY; y++)
			for (int x = 0; x < errorMatrices[layersCount - 1][m]->matrixSizeX; x++)
			{
				NetMatrix* matrix = matrices[layersCount - 1][m];
				int outputID = x + y * matrix->matrixSizeX + matrix->matrixSizeX * matrix->matrixSizeY * m; // Here may be an error
				errorMatrices[layersCount - 1][m]->matrix[x][y] = 2 * (matrix->matrix[x][y] - output[outputID]) * matrix->matrix[x][y] * (1 - matrix->matrix[x][y]);
			}
	}

	for (int l = layersCount - 2; l >= 0; l--)
	{

			for (int m0 = 0; m0 < matricesCount[l]; m0++)
			{	
				int childMatricesCount = getChildMatricesCount(l, m0);

				int childsLayer = 0;
				int firstChildMatrixID = 0;

				getChildMatrix(l, m0, 0, &childsLayer, &firstChildMatrixID);

				NetMatrix* childMatrix = getChildMatrix(l, m0, 0);

				for (int y = 0; y < errorMatrices[l][m0]->matrixSizeY; y++)
					for (int x = 0; x < errorMatrices[l][m0]->matrixSizeX; x++)
					{
						errorMatrices[l][m0]->matrix[x][y] = 0;
						for (int m1 = firstChildMatrixID; m1 < firstChildMatrixID + childMatricesCount; m1++)
						{
							int kernelLeftUpInMatrixX = x - errorMatrices[childsLayer][m1]->kernelOriginX;
							int kernelLeftUpInMatrixY = y - errorMatrices[childsLayer][m1]->kernelOriginY;

							for (int ky = kernelLeftUpInMatrixY; ky < kernelLeftUpInMatrixY + errorMatrices[childsLayer][m1]->kernelSize; ky++)
								for (int kx = kernelLeftUpInMatrixX; kx < kernelLeftUpInMatrixX + errorMatrices[childsLayer][m1]->kernelSize; kx++)
								{
									if (kx >= 0 && kx < errorMatrices[childsLayer][m1]->matrixSizeX)
										if (ky >= 0 && ky < errorMatrices[childsLayer][m1]->matrixSizeY)
										{
												int kernelX = kx - kernelLeftUpInMatrixX;
												int kernelY = ky - kernelLeftUpInMatrixY;

												int weight = branching[l] > 0 ? matrices[childsLayer][m1]->kernel[kernelX][kernelY] : 1; // Pooling is taken in accaunt
												int childError = errorMatrices[childsLayer][m1]->matrix[kx][ky];
												int neuron = matrices[l][m0]->matrix[x][y];

												errorMatrices[l][m0]->matrix[x][y] += childError * weight * neuron * (1 - neuron);
										}
								}
						}
					}
			}
	}
}

void ConvNeuralNetwork::backPropagation(double** output, const int outputDataSize, double k, double a)
{
	
}

void ConvNeuralNetwork::getParentMatrix(int childLayer, int childMatrixID, int* parentLayer, int* parentMatrixID)
{
	if (childLayer > 0)
		(*parentLayer) = childLayer - 1;
	else
		throw;
	if (branching[(*parentLayer)] > 0)
		(*parentMatrixID) = childMatrixID / branching[(*parentLayer)];
	else
		(*parentMatrixID) = childMatrixID;
}

NetMatrix* ConvNeuralNetwork::getParentMatrix(int childLayer, int childMatrixID)
{
	int parentLayer = 0;
	int parentMatrixID = 0;
	getParentMatrix(childLayer, childMatrixID, &parentLayer, &parentMatrixID);
	return matrices[parentLayer][parentMatrixID];
}

NetMatrix* ConvNeuralNetwork::getParentErrorMatrix(int childLayer, int childMatrixID)
{
	int parentLayer = 0;
	int parentMatrixID = 0;
	getParentMatrix(childLayer, childMatrixID, &parentLayer, &parentMatrixID);
	return errorMatrices[parentLayer][parentMatrixID];
}

int ConvNeuralNetwork::getChildMatricesCount(int l, int m)
{
	return branching[l];
}

void ConvNeuralNetwork::getChildMatrix(int l, int m, int childID, int* childLayer, int* childMatrixID)
{
	if (l < 0 || m < 0 || childID < 0) throw;
	if (l + 1 >= layersCount || m >= matricesCount[l]) throw;

	int childMatricesCount = getChildMatricesCount(l, m);
	childMatricesCount = (childMatricesCount > 0 ? childMatricesCount : 1);

	if (childID >= childMatricesCount) throw;

	(*childLayer) = l + 1;
	(*childMatrixID) = childMatricesCount * m + childID;

	if ((*childMatrixID) >= matricesCount[(*childLayer)]) throw;
}

NetMatrix* ConvNeuralNetwork::getChildMatrix(int l, int m, int childID)
{
	int childLayer = 0;
	int childMatrixID = 0;
	getChildMatrix(l, m, childID, &childLayer, &childMatrixID);
	return matrices[childLayer][childMatrixID];
}

NetMatrix* ConvNeuralNetwork::getChildErrorMatrix(int l, int m, int childID)
{
	int childLayer = 0;
	int childMatrixID = 0;
	getChildMatrix(l, m, childID, &childLayer, &childMatrixID);
	return errorMatrices[childLayer][childMatrixID];
}

void ConvNeuralNetwork::setAllWeightsRandom(int seed, int leftEdge, int rightEdge, int accuracy)
{
	for (int l = 0; l < layersCount; l++)
		for (int m = 0; m < matricesCount[l]; m++)
			matrices[l][m]->setRandomWeights(seed, leftEdge, rightEdge, accuracy);
}
