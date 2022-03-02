#include "pch.h"
#include "NetMatrix.h"

NetMatrix::NetMatrix(int matrixSizeX, int matrixSizeY, int kernelSize, int kernelStride, int kernelOriginX, int kernelOriginY)
{
	this->matrixSizeX = matrixSizeX;
	this->matrixSizeY = matrixSizeY;

	matrix = new double* [this->matrixSizeX];
	for (int i = 0; i < this->matrixSizeX; i++)
	{
		matrix[i] = new double[this->matrixSizeY];
		for (int j = 0; j < this->matrixSizeY; j++)
			matrix[i][j] = 0;
	}


	this->kernelSize = kernelSize;
	this->kernelOriginX = kernelOriginX;
	this->kernelOriginY = kernelOriginY;
	this->kernelStride = kernelStride;

	if (kernelSize > 0 && kernelOriginX > 0 && kernelOriginY > 0 && kernelStride > 0)
	{
		kernel = new double* [this->kernelSize];
		for (int i = 0; i < this->kernelSize; i++)
		{
			kernel[i] = new double[this->kernelSize];
			for (int j = 0; j < this->kernelSize; j++)
				kernel[i][j] = 0;
		}
	}
	else
		kernel = nullptr;
}

void NetMatrix::clear(double value)
{
	for (int y = 0; y < matrixSizeY; y++)
		for (int x = 0; x < matrixSizeX; x++)
			matrix[x][y] = value;
}


double NetMatrix::sigmoid(double x)
{
	return 1 / (1 + pow(e, -x));
}


void NetMatrix::unpool(NetMatrix* dest, int poolSizeX, int poolSizeY)
{
	for (int y = 0; y < dest->matrixSizeY; y++)
		for (int x = 0; x < dest->matrixSizeX; x++)
		{
			int xID = x / poolSizeX;
			int yID = y / poolSizeY;

			if (xID >= matrixSizeX) xID = matrixSizeX - 1;
			if(yID >= matrixSizeY) yID = matrixSizeY - 1;

			if (xID < 0) xID = 0;
			if (yID < 0) yID = 0;

			dest->matrix[x][y] = matrix[xID][yID];
		}
}

void NetMatrix::deconvolute(NetMatrix* dest)
{
	for (int y = 0; y < matrixSizeX; y += kernelStride)
		for (int x = 0; x < matrixSizeY; x += kernelStride)
		{
			int kernelLeftUpX = x - kernelOriginX;
			int kernelLeftUpY = y - kernelOriginY;

			for (int kernelY = 0; kernelY < kernelSize; kernelY++)
				for (int kernelX = 0; kernelX < kernelSize; kernelX++)
				{
					int posX = kernelLeftUpX + kernelX;
					int posY = kernelLeftUpY + kernelY;

					if (posX >= 0 && posY >= 0 && posX < matrixSizeX && posY < matrixSizeY)
					{
						//if (posX == 1 && posY == 1)
						//	int point = 0;
						dest->matrix[posX][posY] += matrix[x][y] * kernel[kernelX][kernelY];
					}
						
				}
		}

	for (int y = 0; y < dest->matrixSizeX; y++)
		for (int x = 0; x < dest->matrixSizeY; x++)
			dest->matrix[x][y] = sigmoid(dest->matrix[x][y]);
}


