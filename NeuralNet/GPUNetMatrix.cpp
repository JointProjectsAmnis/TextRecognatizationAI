#include "GPUNetMatrix.h"

GPUNetMatrix::GPUNetMatrix(Graphics* graphics, int matrixSizeX, int matrixSizeY, int kernelSize, int kernelStride, int kernelOriginX, int kernelOriginY)
{
	this->matrixSizeX = matrixSizeX;
	this->matrixSizeY = matrixSizeY;

	matrix = new Texture(graphics, matrixSizeX, matrixSizeY, matricesChannelsCount, matricesFormat);

	//matrix = new double* [this->matrixSizeX];
	//for (int i = 0; i < this->matrixSizeX; i++)
	//{
	//	matrix[i] = new double[this->matrixSizeY];
	//	for (int j = 0; j < this->matrixSizeY; j++)
	//		matrix[i][j] = 0;
	//}


	this->kernelSize = kernelSize;
	this->kernelOriginX = kernelOriginX;
	this->kernelOriginY = kernelOriginY;
	this->kernelStride = kernelStride;

	if (kernelSize > 0 && kernelOriginX > 0 && kernelOriginY > 0 && kernelStride > 0)
	{
		/*kernel = new double* [this->kernelSize];
		oldDeltaKernel = new double* [this->kernelSize];
		for (int i = 0; i < this->kernelSize; i++)
		{
			kernel[i] = new double[this->kernelSize];
			oldDeltaKernel[i] = new double[this->kernelSize];
			for (int j = 0; j < this->kernelSize; j++)
			{
				kernel[i][j] = 0;
				oldDeltaKernel[i][j] = 0;
			}
		}*/
		kernel = new Texture(graphics, kernelSize, kernelSize, matricesChannelsCount, matricesFormat);
		oldDeltaKernel = new Texture(graphics, kernelSize, kernelSize, matricesChannelsCount, matricesFormat);
	}
	else
		kernel = nullptr;
}

GPUNetMatrix::~GPUNetMatrix()
{
	delete matrix;
	delete kernel;
	delete oldDeltaKernel;
}
