#pragma once
#include "Decl.h"
#include <math.h>
#define e 2.7182818284

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
	DECL void clear(double value);
	DECL double sigmoid(double x);

	DECL NetMatrix pool();

	DECL void unpool(NetMatrix* dest, int poolSizeX, int poolSizeY);
	DECL void deconvolute(NetMatrix* dest);


};