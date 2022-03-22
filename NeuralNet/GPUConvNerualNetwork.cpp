#include "GPUConvNerualNetwork.h"

GPUConvNerualNetwork::GPUConvNerualNetwork(Graphics* graphics, mint2 inputMatrixSize, int layersCount, GPUConvNeuralNetDesc netDesc)
{
	shaderForwardProgation = new ShaderCompute(graphics, L"NeuralNetworkShaders/ShaderConvForwardPropagation.hlsl");

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
	matricesSize = new mint2[layersCount];
	matricesSize[0] = inputMatrixSize;


	matrices = new GPUNetMatrix * *[layersCount];
	matricesTextures = new ID3D11UnorderedAccessView * *[layersCount];
	matricesCount = new int[layersCount];
	for (int l = 0; l < layersCount; l++)
	{
		if (l == 0)
		{
			matricesCount[l] = 1;
			matrices[l] = new GPUNetMatrix * [matricesCount[l]];
			matricesTextures[l] = new ID3D11UnorderedAccessView * [matricesCount[l]];

			matrices[l][0] = new GPUNetMatrix(graphics, inputMatrixSize.x, inputMatrixSize.y, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
			matricesTextures[l][0] = matrices[l][0]->matrix->unorderedView;
		}
		else
		{
			if (branching[l - 1] > 0)
			{
				matricesCount[l] = branching[l - 1] * matricesCount[l - 1];
				matrices[l] = new GPUNetMatrix * [matricesCount[l]];
				matricesTextures[l] = new ID3D11UnorderedAccessView * [matricesCount[l]];

				int matrixSizeX = matrices[l - 1][0]->matrixSizeX;
				int matrixSizeY = matrices[l - 1][0]->matrixSizeY;
				for (int m = 0; m < matricesCount[l]; m++)
				{
					matrices[l][m] = new GPUNetMatrix(graphics, matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
					matricesTextures[l][m] = matrices[l][m]->matrix->unorderedView;
				}
			}
			else
			{
				matricesCount[l] = matricesCount[l - 1];
				matrices[l] = new GPUNetMatrix * [matricesCount[l]];
				matricesTextures[l] = new ID3D11UnorderedAccessView * [matricesCount[l]];

				int matrixSizeX = ceil(matrices[l - 1][0]->matrixSizeX / (double)poolingSize[l].x);
				int matrixSizeY = ceil(matrices[l - 1][0]->matrixSizeY / (double)poolingSize[l].y);
				for (int m = 0; m < matricesCount[l]; m++)
				{
					matrices[l][m] = new GPUNetMatrix(graphics, matrixSizeX, matrixSizeY, kernelSize[l], kernelStride[l], kernelOrigin[l].x, kernelOrigin[l].y);
					matricesTextures[l][m] = matrices[l][m]->matrix->unorderedView;
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

GPUConvNerualNetwork::~GPUConvNerualNetwork()
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
	delete shaderForwardProgation;
}

void GPUConvNerualNetwork::forwardPropagation(Graphics* graphics,double** inputData, const int inputDataSize)
{
	int outputSizeX = matrices[1][0]->matrixSizeX;
	int outputSizeY = matrices[1][0]->matrixSizeY;

	Texture* textureResult = new Texture(graphics, outputSizeX, outputSizeY, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_READ);

	shaderForwardProgation->BindShader();
	//graphics->context->CSSetUnorderedAccessViews(0, 1, &(matricesTextures[0][0]), nullptr);
	UINT UAVsCount = matricesCount[1];
	graphics->context->CSSetUnorderedAccessViews(0, UAVsCount, matricesTextures[1], &UAVsCount);
	//graphics->context->CSSetUnorderedAccessViews(1, 1, &(matricesTextures[1][0]), nullptr);
	graphics->Dispatch(ceil((matricesCount[0] * matricesSize[0].x) / 8.0f), ceil((matricesCount[0] * matricesSize[0].y) / 8.0f), 1);

	graphics->context->CopyResource(textureResult->texture, matrices[1][1]->matrix->texture);


	mfloat4** buffer = new mfloat4 * [outputSizeX];
	for (int x = 0; x < outputSizeX; x++)
	{
		buffer[x] = new mfloat4[outputSizeY];
		for (int y = 0; y < outputSizeY; y++)
			buffer[x][y] = { 0.5f, 0.5f, 0.5f, 1 };
	}


	D3D11_MAPPED_SUBRESOURCE sb;
	HRESULT hr = graphics->context->Map(textureResult->texture, 0, D3D11_MAP_READ, 0, &sb);
	if (FAILED(hr)) throw;

	for (int y = 0; y < outputSizeX; y++)
		for (int x = 0; x < outputSizeY; x++)
			buffer[x][y] = *((mfloat4*)sb.pData + x + y * (outputSizeX));


	for (int y = 0; y < outputSizeY; y++)
	{
		for (int x = 0; x < outputSizeX; x++)
			std::cout << buffer[x][y].x << " ";
		std::cout << std::endl;
	}

	delete textureResult;
}
