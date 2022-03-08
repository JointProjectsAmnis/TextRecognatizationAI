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
		oldDeltaKernel = new double*[this->kernelSize];
		for (int i = 0; i < this->kernelSize; i++)
		{
			kernel[i] = new double[this->kernelSize];
			oldDeltaKernel[i] = new double[this->kernelSize];
			for (int j = 0; j < this->kernelSize; j++)
			{
				kernel[i][j] = 0;
				oldDeltaKernel[i][j] = 0;
			}
		}
	}
	else
		kernel = nullptr;
}

NetMatrix::~NetMatrix()
{
	for (int x = 0; x < matrixSizeX; x++)
		delete[matrixSizeY] matrix[x];
	delete[matrixSizeX] matrix;

	for (int x = 0; x < kernelSize; x++)
		delete[kernelSize] kernel[x];
	delete[kernelSize] kernel;
}

void NetMatrix::clear(double value)
{
	for (int y = 0; y < matrixSizeY; y++)
		for (int x = 0; x < matrixSizeX; x++)
			matrix[x][y] = value;
}


double NetMatrix::sigmoid(double x)
{
	return 1 / (1 + pow(eiler, -x));
}

void NetMatrix::setRandomWeights(int seed, int module, double devider, double offset)
{
	srand(seed);
	for(int y = 0; y < kernelSize; y++)
		for (int x = 0; x < kernelSize; x++)
			kernel[x][y] = ((rand() % module) / devider) + offset;
}

void NetMatrix::setRandomWeights(int seed, int leftEdge, int rightEdge, int accuracy)
{
	if (leftEdge > rightEdge) throw;

	int length = rightEdge - leftEdge;
	srand(seed);
	for (int y = 0; y < kernelSize; y++)
		for (int x = 0; x < kernelSize; x++)
		{
			int randomValue = rand();
			kernel[x][y] = ((randomValue % (length * accuracy)) / (double)accuracy) + leftEdge;
		}
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
	for (int y = 0; y < matrixSizeY; y += kernelStride)
		for (int x = 0; x < matrixSizeX; x += kernelStride)
		{
			int kernelLeftUpX = x - kernelOriginX;
			int kernelLeftUpY = y - kernelOriginY;

			for (int kernelY = 0; kernelY < kernelSize; kernelY++)
				for (int kernelX = 0; kernelX < kernelSize; kernelX++)
				{
					int posX = kernelLeftUpX + kernelX;
					int posY = kernelLeftUpY + kernelY;

					if (posX >= 0 && posY >= 0 && posX < matrixSizeX && posY < matrixSizeY)
						dest->matrix[posX][posY] += matrix[x][y] * kernel[kernelX][kernelY];	
				}
		}

	for (int y = 0; y < dest->matrixSizeY; y++)
		for (int x = 0; x < dest->matrixSizeX; x++)
			dest->matrix[x][y] = sigmoid(dest->matrix[x][y]);
}


