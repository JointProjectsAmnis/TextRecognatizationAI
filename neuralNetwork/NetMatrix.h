#pragma once
#include "Decl.h"

class NetMatrix
{
public:
	int matrixSizeX;
	int matrixSizeY;

	int kernelSize;
	int kernelStride;
	int kernelOriginX;
	int kernelOriginY;

	double** matrix;
	double** kernel;

	DECL NetMatrix(int matrixSizeX, int matrixSizeY, int kernelSize = -1, int kernelStride = -1, int kernelOriginX = -1, int kernelOriginY = -1);

	DECL NetMatrix pool();

	DECL void unpool(NetMatrix* dest, int poolSizeX, int poolSizeY);
	DECL void deconvolute(NetMatrix* dest);
};