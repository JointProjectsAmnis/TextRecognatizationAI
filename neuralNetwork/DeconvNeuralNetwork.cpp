#include "pch.h"
#include "DeconvNeuralNetwork.h"

DeconvNeuralNetwork::DeconvNeuralNetwork(int2 outputImageSize, int layersCount, DevconvNeuralNetDesc netDesc)
{

	if (outputImageSize.x <= 0 || outputImageSize.y <= 0) throw;
	this->outputImageSize = outputImageSize;

	if (layersCount <= 0) throw;
	this->layersCount = layersCount;

	if (!netDesc.branching)	throw;
	this->branching = netDesc.branching;


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

	unpoolingSize = new int2[layersCount];
	if (netDesc.unpoolingSize)
		memcpy(unpoolingSize, netDesc.unpoolingSize, layersCount * sizeof(int2));
	else
	{
		if (netDesc.defaultUnpoolingSize.x > 0 && netDesc.defaultUnpoolingSize.y > 0)
		{
			defaultUnpoolingSize = netDesc.defaultUnpoolingSize;
			for (int i = 0; i < layersCount; i++)
				unpoolingSize[i] = defaultUnpoolingSize;
		}
		else
			throw;
	}

	matrices = new NetMatrix * *[layersCount];
	matricesCount = new int[layersCount];
	for (int l = layersCount - 1; l >= 0; l--)
	{
		if (l == layersCount - 1)
		{
			matricesCount[l] = 1;
			matrices[l] = new NetMatrix * [matricesCount[l]];
			matrices[l][0] = new NetMatrix(outputImageSize.x, outputImageSize.y, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
		}
		else
		{
			if (branching[l + 1] > 0)
			{
				matricesCount[l] = branching[l + 1] * matricesCount[l + 1];
				matrices[l] = new NetMatrix * [matricesCount[l]];
				int matrixSizeX = matrices[l + 1][0]->matrixSizeX;
				int matrixSizeY = matrices[l + 1][0]->matrixSizeY;
				for (int m = 0; m < matricesCount[l]; m++)
					matrices[l][m] = new NetMatrix(matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
			}
			else
			{
				matricesCount[l] = matricesCount[l + 1];
				matrices[l] = new NetMatrix * [matricesCount[l]];

				int matrixSizeX = ceil(matrices[l + 1][0]->matrixSizeX / (double)unpoolingSize[l].x);
				int matrixSizeY = ceil(matrices[l + 1][0]->matrixSizeY / (double)unpoolingSize[l].y);
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


void DeconvNeuralNetwork::forwardPropagation(void* input, const int inputDataSize)
{
	int inputMatricesDataCount = ((matrices[0][0]->matrixSizeX * matrices[0][0]->matrixSizeY) * matricesCount[0]) * sizeof(matrices[0][0]->matrix[0][0]);
	if (inputMatricesDataCount != inputDataSize) throw;

	for (int m = 0; m < matricesCount[0]; m++)
	{
		for (int y = 0; y < matrices[0][m]->matrixSizeY; y++)
			for (int x = 0; x < matrices[0][m]->matrixSizeX; x++)
			{
				int id = x + y * matrices[0][m]->matrixSizeX + m * matrices[0][m]->matrixSizeX * matrices[0][m]->matrixSizeY;
				matrices[0][m]->matrix[x][y] = ((double*)input)[id];
			}
	}

	for (int l = 0; l < layersCount - 1; l++)
		for (int m = 0; m < matricesCount[l]; m++)
		{
			if (branching[l + 1] <= 0)
				matrices[l][m]->unpool(matrices[l + 1][m], unpoolingSize[l + 1].x, unpoolingSize[l + 1].y);
			else
			{
				int nextLayerMatrixID = m / branching[l + 1];
				matrices[l][m]->deconvolute(matrices[l + 1][nextLayerMatrixID]);
			}
		}
}


void DeconvNeuralNetwork::calculateErrors(double** output)
{
	for (int l = layersCount - 1; l >= 0; l--)
		for (int m = 0; m < matricesCount[l]; m++)
		{
			for (int y = 0; y < errorMatrices[l][m]->matrixSizeY; y++)
				for (int x = 0; x < errorMatrices[l][m]->matrixSizeX; x++)
				{
					if (l == layersCount - 1)
					{
						errorMatrices[l][m]->matrix[x][y] = 2 * (matrices[l][m]->matrix[x][y] - output[x][y]) * matrices[l][m]->matrix[x][y] * (1 -
							matrices[l][m]->matrix[x][y]);
					}
					else
					{
						if (branching[l + 1] > 0)
						{
							NetMatrix* parentErrorMatrix = getParentErrorMatrix(l, m);
							int kernelOriginX = x;
							int kernelOriginY = y;
							int kernelLeftUpX = x - matrices[l][m]->kernelOriginX;
							int kernelLeftUpY = y - matrices[l][m]->kernelOriginY;

							errorMatrices[l][m]->matrix[x][y] = 0;
							for (int posY = kernelLeftUpY; posY < kernelLeftUpY + matrices[l][m]->kernelSize; posY++)
								for (int posX = kernelLeftUpX; posX < kernelLeftUpX + matrices[l][m]->kernelSize; posX++)
								{
									if (posX >= 0 && posX < matrices[l][m]->matrixSizeX)
										if (posY >= 0 && posY < matrices[l][m]->matrixSizeY)
										{
											int posXInKernel = posX - kernelLeftUpX;
											int posYInKernel = posY - kernelLeftUpY;
											if (branching[l] > 0)
											{
												errorMatrices[l][m]->matrix[x][y] += parentErrorMatrix->matrix[posX][posY] * matrices[l][m]->kernel[posXInKernel][posYInKernel] * matrices[l][m]->matrix[x][y] * (1 - matrices[l][m]->matrix[x][y]);
											}
											else
											{
												errorMatrices[l][m]->matrix[x][y] += parentErrorMatrix->matrix[posX][posY] * matrices[l][m]->kernel[posXInKernel][posYInKernel];
											}
										}
								}
						}
						else
						{
							NetMatrix* parentErrorMatrix = getParentErrorMatrix(l, m);
							int unpoolLeftUpCornerX = x * unpoolingSize[l + 1].x;
							int unpoolLeftUpCornerY = y * unpoolingSize[l + 1].y;

							for (int unpoolY = unpoolLeftUpCornerY; unpoolY < unpoolingSize[l + 1].y; unpoolY++)
								for (int unpoolX = unpoolLeftUpCornerX; unpoolX < unpoolingSize[l + 1].x; unpoolX++)
								{
									if (unpoolX < parentErrorMatrix->matrixSizeX && unpoolY < parentErrorMatrix->matrixSizeY)
									{
										errorMatrices[l][m]->matrix[x][y] += parentErrorMatrix->matrix[unpoolX][unpoolY] * 1 * matrices[l][m]->matrix[x][y] * (1 - matrices[l][m]->matrix[x][y]);
									}
								}
						}
					}
				}
		}
}


void DeconvNeuralNetwork::correctWeights(double k, double a)
{
	for (int l = 0; l < layersCount - 1; l++)
		if (branching[l + 1] > 0)
			for (int m = 0; m < matricesCount[l]; m++)
			{
				NetMatrix* parentErrorMatrix = getParentErrorMatrix(l, m);
				for (int kx = 0; kx < matrices[l][m]->kernelSize; kx++)
					for (int ky = 0; ky < matrices[l][m]->kernelSize; ky++)
					{
						double weightDiff = 0;
						for (int y = 0; y < parentErrorMatrix->matrixSizeY; y++)
							for (int x = 0; x < parentErrorMatrix->matrixSizeX; x++)
							{
								int posXInMatrix = x - matrices[l][m]->kernelOriginX + kx;
								int posYInMatrix = y - matrices[l][m]->kernelOriginY + ky;
								if (posXInMatrix >= 0 && posXInMatrix < parentErrorMatrix->matrixSizeX)
									if (posYInMatrix >= 0 && posYInMatrix < parentErrorMatrix->matrixSizeY)
										weightDiff += parentErrorMatrix->matrix[posXInMatrix][posYInMatrix] * matrices[l][m]->matrix[x][y];
							}
						double weightDelta = weightDiff * k + matrices[l][m]->oldDeltaKernel[kx][ky] * a;
						matrices[l][m]->kernel[kx][ky] -= weightDelta;
						matrices[l][m]->oldDeltaKernel[kx][ky] = weightDelta;
					}
			}
}

void DeconvNeuralNetwork::backPropagation(double** output, const int outputDataSize, double k, double a)
{
	int lastMatrixSize = matrices[layersCount - 1][0]->matrixSizeX * matrices[layersCount - 1][0]->matrixSizeY * sizeof(double);
	if (lastMatrixSize != outputDataSize) throw;
	calculateErrors(output);
	correctWeights(k, a);
}

void DeconvNeuralNetwork::setAllWeightsRandom(int seed, int leftEdge, int rightEdge, int accuracy)
{
	for(int l = 0; l < layersCount; l++)
		for (int m = 0; m < matricesCount[l]; m++)
			matrices[l][m]->setRandomWeights(seed, leftEdge, rightEdge, accuracy);
}


void DeconvNeuralNetwork::getParentMatrix(int childLayer, int childMatrixID, int* parentLayer, int* parentMatrixID)
{
	*parentLayer = childLayer + 1;
	*parentMatrixID = -1;
	if (branching[*parentLayer] > 0)
		*parentMatrixID = childMatrixID / branching[*parentLayer];
	else
		*parentMatrixID = childMatrixID;
}

NetMatrix* DeconvNeuralNetwork::getParentMatrix(int childLayer, int childMatrixID)
{
	int parentLayer = 0;
	int parentMatrixID = 0;
	getParentMatrix(childLayer, childMatrixID, &parentLayer, &parentMatrixID);
	return matrices[parentLayer][parentMatrixID];
}

NetMatrix* DeconvNeuralNetwork::getParentErrorMatrix(int childLayer, int childMatrixID)
{
	int parentLayer = 0;
	int parentMatrixID = 0;
	getParentMatrix(childLayer, childMatrixID, &parentLayer, &parentMatrixID);
	return errorMatrices[parentLayer][parentMatrixID];
}

double DeconvNeuralNetwork::getError(double** output, const int outputDataSize)
{
	int lastMatrixSize = matrices[layersCount - 1][0]->matrixSizeX * matrices[layersCount - 1][0]->matrixSizeY * sizeof(double);
	if (lastMatrixSize != outputDataSize) throw;

	double error = 0;
	int valuesCount = matrices[layersCount - 1][0]->matrixSizeY * matrices[layersCount - 1][0]->matrixSizeX;

	for(int y = 0; y < matrices[layersCount - 1][0]->matrixSizeY; y++)
		for (int x = 0; x < matrices[layersCount - 1][0]->matrixSizeX; x++)
			error += pow(output[x][y] - matrices[layersCount - 1][0]->matrix[x][y], 2) / valuesCount;

	return error;
}

DeconvNeuralNetwork::~DeconvNeuralNetwork()
{
	if(branching) delete[layersCount] branching;
	if(kernelSize) delete[layersCount] kernelSize;
	if(kernelStride) delete[layersCount] kernelStride;
	if(kernelOrigin) delete[layersCount] kernelOrigin;
	if(unpoolingSize) delete[layersCount] unpoolingSize;

	// Deleting matrices

	for (int l = 0; l < layersCount; l++)
		for (int m = 0; m < matricesCount[l]; m++)
			if (matrices[l][m]) delete matrices[l][m];
	for (int l = 0; l < layersCount; l++)
		if (matrices[l]) delete[matricesCount[l]] matrices[l];
	if(matrices) delete[layersCount] matrices;

	// Deleting errorMatrices

	for (int l = 0; l < layersCount; l++)
		for (int m = 0; m < matricesCount[l]; m++)
			if (errorMatrices[l][m]) delete errorMatrices[l][m];
	for (int l = 0; l < layersCount; l++)
		if (errorMatrices[l]) delete[matricesCount[l]] errorMatrices[l];
	if (errorMatrices) delete[layersCount] errorMatrices;

	if(matricesCount) delete[layersCount] matricesCount;
}
