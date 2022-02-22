#include "pch.h"
#include "NetMatrix.h"

NetMatrix::NetMatrix(int matrixSizeX, int matrixSizeY, int kernelSize, int kernelStride, int kernelOriginX, int kernelOriginY)
{
	this->matrixSizeX = matrixSizeX;
	this->matrixSizeY = matrixSizeY;

	matrix = new double* [this->matrixSizeX];
	for (int i = 0; i < this->matrixSizeX; i++)
		matrix[i] = new double[this->matrixSizeY];

	this->kernelSize = kernelSize;
	this->kernelOriginX = kernelOriginX;
	this->kernelOriginY = kernelOriginY;
	this->kernelStride = kernelStride;

	if (kernelSize > 0 && kernelOriginX > 0 && kernelOriginY > 0 && kernelStride > 0)
	{
		kernel = new double* [this->kernelSize];
		for (int i = 0; i < this->kernelSize; i++)
			matrix[i] = new double[this->kernelSize];
	}
	else
		kernel = nullptr;
}

void NetMatrix::unpool(NetMatrix* dest, int poolSizeX, int poolSizeY)
{
	for (int y = 0; y < dest->matrixSizeX; y++)
		for (int x = 0; x < dest->matrixSizeY; x++)
		{
			int xID = x / poolSizeX;
			int yID = y / poolSizeY;
			dest->matrix[x][y] = matrix[xID][yID];
		}
}

void NetMatrix::deconvolute(NetMatrix* dest)
{
	for (int y = 0; y < matrixSizeX; y++)
		for (int x = 0; x < matrixSizeY; y++)
		{
			int kernelLeftUpX = x - kernelOriginX;
			int kernelLeftUpY = y - kernelOriginY;

			for (int kernelY = 0; kernelY < kernelSize; kernelY++)
				for (int kernelX = 0; kernelX < kernelSize; kernelX++)
				{
					int posX = kernelLeftUpX + kernelX;
					int posY = kernelLeftUpY + kernelY;

					if (posX >= 0 && posY >= 0)
						dest->matrix[posX][posY] += matrix[x][y] * kernel[kernelX][kernelY];
				}
		}

	// Need to add sigmoidation

}
