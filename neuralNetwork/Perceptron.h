#pragma once
#include "Decl.h"
#include <math.h>
#include <stdlib.h>

class Perceptron
{
public:
	int layersCount;
	int* neuronsCountInLayer;
	float** neurons;
	float** neuronsE;
	float*** weights;
	float*** oldDiff;
	int biosesInLayerCount = 1;
	float biosesValue = 1;

	struct Sample
	{
		float* inputData;
		float* outputData;
	};

	DECL Perceptron(int layersCount, int* neuronsInLayerCount);
	DECL ~Perceptron();
	DECL double sigmoid(double x);
	DECL void forwardPropagation(float* inputData);
	DECL float backPropagation(float* t, float a, float k);
	DECL void use(float* inputData, float* outputData);
	DECL float getError(float* t);
	DECL float getTotalError(Sample* samples, int samplesCount);
private:
	DECL float updateErrors(float* t);
};

