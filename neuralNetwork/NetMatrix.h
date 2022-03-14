#pragma once
#include "Decl.h"
#include <math.h>
#include <wincrypt.h>
#include <stdlib.h>
#define eiler 2.7182818284

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
	double** oldDeltaKernel;

	DECL NetMatrix(int matrixSizeX, int matrixSizeY, int kernelSize = -1, int kernelStride = -1, int kernelOriginX = -1, int kernelOriginY = -1);
	DECL ~NetMatrix();
	DECL void clear(double value);
	DECL double sigmoid(double x);
	DECL void setRandomWeights(int seed, int module, double devider, double offset);
	DECL void setRandomWeights(int seed, int leftEdge, int rightEdge, int accuracy);
	DECL void fill(double** data);


	DECL void pool(NetMatrix* dest, int poolSizeX, int poolSizeY);
	DECL void convolute(NetMatrix* dest);

	DECL void unpool(NetMatrix* dest, int poolSizeX, int poolSizeY);
	DECL void deconvolute(NetMatrix* dest);


};