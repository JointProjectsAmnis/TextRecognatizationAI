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

void DeconvNeuralNetwork::calculateErrors(double** output)
{
	for(int l = layersCount - 1; l >= 0; l--)
		for (int m = 0; m < matricesCount[l]; m++)
		{
			for(int y = 0; y < errorMatrices[l][m]->matrixSizeY; y++)
				for (int x = 0; x < errorMatrices[l][m]->matrixSizeX; x++)
				{
					if (l == layersCount - 1)
					{
						errorMatrices[l][m]->matrix[x][y] = 2 * (matrices[l][m]->matrix[x][y] - output[x][y]) * matrices[l][m]->matrix[x][y] * (1 - 
							matrices[l][m]->matrix[x][y]);
					}
					else
					{
						if (unpoolingLayers[l + 1] <= 0) // In this case may be an error
						{
							NetMatrix* parentErrorMatrix = getParentErrorMatrix(l, m);
							int kernelOriginX = x;
							int kernelOriginY = y;
							int kernelLeftUpX = x - matrices[l][m]->kernelSize;
							int kernelLeftUpY = y - matrices[l][m]->kernelSize;

							errorMatrices[l][m]->matrix[x][y] = 0;
							for(int posX = kernelLeftUpX; posX < kernelLeftUpX + matrices[l][m]->kernelSize; posX++)
								for (int posY = kernelLeftUpY; posY < kernelLeftUpY + matrices[l][m]->kernelSize; posY++)
								{
									if(posX >= 0 && posX < matrices[l][m]->matrixSizeX)
										if (posY >= 0 && posY < matrices[l][m]->matrixSizeY)
										{
											int posXInKernel = posX - kernelLeftUpX;
											int posYInKernel = posY - kernelLeftUpY;
											errorMatrices[l][m]->matrix[x][y] += parentErrorMatrix->matrix[posX][posY] * matrices[l][m]->kernel[posXInKernel][posYInKernel] * matrices[l][m]->matrix[x][y] * (1 - matrices[l][m]->matrix[x][y]);
										}
								}



							//for(int xk = kernelLeftUpX; xk < matrices[l][m]->kernelSize; xk++)
							//	for (int yk = kernelLeftUpY; yk < matrices[l][m]->kernelSize; yk++)
							//	{
							//		if (xk >= 0 && xk < yk >= 0)
							//		{
							//			int posXInKernel = xk - kernelLeftUpX;
							//			int posYInKernel = yk - kernelLeftUpY;
							//			errorMatrices[l][m]->matrix[x][y] += parentErrorMatrix->matrix[xk][yk] * matrices[l][m]->kernel[posXInKernel][posYInKernel] * matrices[l][m]->matrix[x][y] * (1 - matrices[l][m]->matrix[x][y]);
							//		}
							//	}
						}
						else
						{
							NetMatrix* parentErrorMatrix = getParentErrorMatrix(l, m);
							int unpoolLeftUpCornerX = x * unpoolingSize[l + 1].x; // Here may be and error
							int unpoolLeftUpCornerY = y * unpoolingSize[l + 1].y; // Here may be and error

							for(int unpoolX = unpoolLeftUpCornerX; unpoolX < unpoolingSize[l + 1].x; unpoolX++)
								for (int unpoolY = unpoolLeftUpCornerY; unpoolY < unpoolingSize[l + 1].y; unpoolY++)
								{
									errorMatrices[l][m]->matrix[x][y] = parentErrorMatrix->matrix[unpoolX][unpoolY] * 1 * matrices[l][m]->matrix[x][y] * (1 - matrices[l][m]->matrix[x][y]); // Here may be an error
								}
						}
					}
				}
		}
}

void DeconvNeuralNetwork::correctWeights()
{
	for(int l = 0; l < layersCount - 1; l++)
		if(unpoolingLayers[l + 1] > 0)
			for (int m = 0; m < matricesCount[l]; m++)
			{
				double deltaWeight = 0;
				for(int kx = 0; kx < matrices[l][m]->kernelSize; kx++)
					for (int ky = 0; ky < matrices[l][m]->kernelSize; ky++)
					{

					}

			}
}

void DeconvNeuralNetwork::backPropagation(double** output, const int outputDataSize)
{
	int lastMatrixSize = matrices[layersCount - 1][0]->matrixSizeX * matrices[layersCount - 1][0]->matrixSizeY * sizeof(double);
	if (lastMatrixSize != outputDataSize) throw;
	calculateErrors(output);

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
	if (unpoolingLayers[*parentLayer] <= 0)
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
