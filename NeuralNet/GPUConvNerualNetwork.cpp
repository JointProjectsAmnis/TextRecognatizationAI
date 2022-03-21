#include "GPUConvNerualNetwork.h"

GPUConvNerualNetwork::GPUConvNerualNetwork(Graphics* graphics, mint2 inputMatrixSize, int layersCount, ConvNeuralNetDesc netDesc)
{
	if (inputMatrixSize.x <= 0 || inputMatrixSize.y <= 0) throw;
	this->inputMatrixSize = inputMatrixSize;

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

	kernelOrigin = new mint2[layersCount];
	if (netDesc.kernelOrigin)
		memcpy(kernelOrigin, netDesc.kernelOrigin, layersCount * sizeof(mint2));
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

	poolingSize = new mint2[layersCount];
	if (netDesc.poolingSize)
		memcpy(poolingSize, netDesc.poolingSize, layersCount * sizeof(mint2));
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

	matricesSize[0] = inputMatrixSize;


	matrices = new GPUNetMatrix * *[layersCount];
	matricesCount = new int[layersCount];
	for (int l = 0; l < layersCount; l++)
	{
		if (l == 0)
		{
			matricesCount[l] = 1;
			matrices[l] = new GPUNetMatrix * [matricesCount[l]];
			matrices[l][0] = new GPUNetMatrix(graphics, inputMatrixSize.x, inputMatrixSize.y, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
		}
		else
		{
			if (branching[l - 1] > 0)
			{
				matricesCount[l] = branching[l - 1] * matricesCount[l - 1];
				matrices[l] = new GPUNetMatrix * [matricesCount[l]];
				int matrixSizeX = matrices[l - 1][0]->matrixSizeX;
				int matrixSizeY = matrices[l - 1][0]->matrixSizeY;
				for (int m = 0; m < matricesCount[l]; m++)
				{
					matrices[l][m] = new GPUNetMatrix(graphics, matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
				}

			}
			else
			{
				matricesCount[l] = matricesCount[l - 1];
				matrices[l] = new GPUNetMatrix * [matricesCount[l]];

				int matrixSizeX = ceil(matrices[l - 1][0]->matrixSizeX / (double)poolingSize[l].x);
				int matrixSizeY = ceil(matrices[l - 1][0]->matrixSizeY / (double)poolingSize[l].y);
				for (int m = 0; m < matricesCount[l]; m++)
				{
					matrices[l][m] = new GPUNetMatrix(graphics, matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
				}

			}
		}
	}

	errorMatrices = new GPUNetMatrix * *[layersCount];

	for (int l = 0; l < layersCount; l++)
		errorMatrices[l] = new GPUNetMatrix * [matricesCount[l]];

	for (int l = 0; l < layersCount; l++)
	{
		int matrixSizeX = matrices[l][0]->matrixSizeX;
		int matrixSizeY = matrices[l][0]->matrixSizeY;

		for (int m = 0; m < matricesCount[l]; m++)
			errorMatrices[l][m] = new GPUNetMatrix(graphics, matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
	}

	//matrices = new Texture * *[layersCount];
	//kernels = new Texture * *[layersCount];
	//matricesCount = new int[layersCount];
	//for (int l = 0; l < layersCount; l++)
	//{
	//	if (l == 0)
	//	{
	//		matricesCount[l] = 1;
	//		matrices[l] = new Texture * [matricesCount[l]];
	//		//matrices[l][0] = new NetMatrix(inputMatrixSize.x, inputMatrixSize.y, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
	//		matrices[l][0] = new Texture(graphics, inputMatrixSize.x, inputMatrixSize.y, matricesChannelsCount, matricesFormat);
	//	}
	//	else
	//	{
	//		if (branching[l - 1] > 0)
	//		{
	//			matricesCount[l] = branching[l - 1] * matricesCount[l - 1];
	//			//matrices[l] = new NetMatrix * [matricesCount[l]];
	//			matrices[l] = new Texture * [matricesCount[l]];
	//			kernels[l] = new Texture * [matricesCount[l]];
	//			matricesSize[l].x = matricesSize[l - 1].x;
	//			matricesSize[l].y = matricesSize[l - 1].y;


	//			for (int m = 0; m < matricesCount[l]; m++)
	//			{
	//				//matrices[l][m] = new NetMatrix(matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
	//				matrices[l][m] = new Texture(graphics, matricesSize[l].x, matricesSize[l].y, matricesChannelsCount, matricesFormat);
	//				kernels[l][m] = new Texture(graphics, kernelSize[l], kernelSize[l], matricesChannelsCount, matricesFormat);
	//			}

	//		}
	//		else
	//		{
	//			matricesCount[l] = matricesCount[l - 1];
	//			matrices[l] = new Texture * [matricesCount[l]];
	//			kernels[l] = new Texture * [matricesCount[l]];

	//			//int matrixSizeX = ceil(matrices[l - 1][0]->matrixSizeX / (double)poolingSize[l].x);
	//			//int matrixSizeY = ceil(matrices[l - 1][0]->matrixSizeY / (double)poolingSize[l].y);
	//			matricesSize[l].x = ceil(matricesSize[l - 1].x / (double)poolingSize[l].x);
	//			matricesSize[l].y = ceil(matricesSize[l - 1].y / (double)poolingSize[l].y);
	//			for (int m = 0; m < matricesCount[l]; m++)
	//			{
	//				//matrices[l][m] = new NetMatrix(matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
	//				matrices[l][m] = new Texture(graphics, matricesSize[l].x, matricesSize[l].y, matricesChannelsCount, matricesFormat);
	//				kernels[l][m] = new Texture(graphics, kernelSize[l], kernelSize[l], matricesChannelsCount, matricesFormat);
	//			}

	//		}
	//	}
	//}

	//errorMatrices = new NetMatrix * *[layersCount];

	//for (int l = 0; l < layersCount; l++)
	//	errorMatrices[l] = new NetMatrix * [matricesCount[l]];

	//for (int l = 0; l < layersCount; l++)
	//{
	//	int matrixSizeX = matrices[l][0]->matrixSizeX;
	//	int matrixSizeY = matrices[l][0]->matrixSizeY;

	//	for (int m = 0; m < matricesCount[l]; m++)
	//		errorMatrices[l][m] = new NetMatrix(matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
	//}
}
