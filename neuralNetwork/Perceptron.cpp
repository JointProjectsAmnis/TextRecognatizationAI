#include "pch.h"
#include "Perceptron.h"
#define e 2.7182818284

Perceptron::Perceptron(int layersCount, int* neuronsInLayerCount)
{
	this->layersCount = layersCount;
	this->neuronsCountInLayer = new int[layersCount];

	for (int l = 0; l < layersCount; l++)
		this->neuronsCountInLayer[l] = neuronsInLayerCount[l] + biosesInLayerCount;

	neurons = new float* [this->layersCount];
	neuronsE = new float* [this->layersCount];
	for (int l = 0; l < this->layersCount; l++)
	{
		neurons[l] = new float[this->neuronsCountInLayer[l]];
		neuronsE[l] = new float[this->neuronsCountInLayer[l]];

		for (int i = 0; i < this->neuronsCountInLayer[l]; i++)
		{
			if (i != this->neuronsCountInLayer[l] - 1)
				neurons[l][i] = 0; // Here you can set the initial value for neurons
			else
				neurons[l][i] = biosesValue;
			neuronsE[l][i] = 0; // Here you can set the initial value for neuronsE
		}
	}

	weights = new float** [this->layersCount];
	for (int l = 1; l < this->layersCount; l++)
	{
		weights[l] = new float* [this->neuronsCountInLayer[l]];
		for (int n1 = 0; n1 < this->neuronsCountInLayer[l] - biosesInLayerCount; n1++)
		{
			weights[l][n1] = new float[this->neuronsCountInLayer[l - 1]];

			for (int n0 = 0; n0 < this->neuronsCountInLayer[l - 1]; n0++)
			{
				//weights[l][n1][n0] = 0.1 * (n0 + 1) + 0.1 * n1 * this->neuronsCountInLayer[l - 1];
				weights[l][n1][n0] = ((rand() % 1000) / 1000.0) * 2 - 1; 	// Here you can set the initial value for weights
				//weights[l][n1][n0] = 0.5f; 	// Here you can set the initial value for weights
			}
		}
	}

	oldDiff = new float** [this->layersCount];
	for (int l = 1; l < this->layersCount; l++)
	{
		oldDiff[l] = new float* [this->neuronsCountInLayer[l]];
		for (int n1 = 0; n1 < this->neuronsCountInLayer[l] - biosesInLayerCount; n1++)
		{
			oldDiff[l][n1] = new float[this->neuronsCountInLayer[l - 1]];

			for (int n0 = 0; n0 < this->neuronsCountInLayer[l - 1]; n0++)
			{
				oldDiff[l][n1][n0] = 0; 	// Here you can set the initial value for oldDiff
			}
		}
	}
}

Perceptron::~Perceptron()
{
	for (int l = 1; l < this->layersCount; l++)
	{
		for (int n1 = 0; n1 < this->neuronsCountInLayer[l] - biosesInLayerCount; n1++)
			delete[this->neuronsCountInLayer[l - 1]] weights[l][n1];

		delete[this->neuronsCountInLayer[l] - biosesInLayerCount] weights[l];
	}
	delete[this->layersCount] weights;

	for (int l = 0; l < this->layersCount; l++)
		delete[this->neuronsCountInLayer[l]] neurons[l];
	delete[this->layersCount] neurons;

	delete[layersCount] neuronsCountInLayer;
}

double Perceptron::sigmoid(double x)
{
	return (1 / (1 + pow(e, -x)));
}

float Perceptron::updateErrors(float* t)
{
	float error = getError(t);

	for (int l = layersCount - 1; l >= 0; l--)
	{
		if (l == layersCount - 1)
		{
			for (int n1 = 0; n1 < neuronsCountInLayer[l]; n1++)
			{
				neuronsE[l][n1] = 2 * (neurons[l][n1] - t[n1]) * neurons[l][n1] * (1 - neurons[l][n1]);
			}
		}
		else
		{
			for (int n1 = 0; n1 < neuronsCountInLayer[l]; n1++)
			{
				if (l == 0 && n1 == 0)
					int point = 0;
				neuronsE[l][n1] = 0;
				for (int n2 = 0; n2 < neuronsCountInLayer[l + 1] - biosesInLayerCount; n2++)
				{
					//neuronsE[l][n1] += neuronsE[l + 1][n2] * weights[l + 1][n2][n1] * neurons[l][n1] * (1 - neurons[l][n1]);
					neuronsE[l][n1] += neuronsE[l + 1][n2] * weights[l + 1][n2][n1];
				}

				neuronsE[l][n1] *= neurons[l][n1] * (1 - neurons[l][n1]);
			}
		}
	}
	return error;
}

void Perceptron::frontPropagation(float* inputData)
{
	for (int n = 0; n < neuronsCountInLayer[0] - biosesInLayerCount; n++)
		neurons[0][n] = inputData[n];

	for (int l = 1; l < layersCount; l++)
		for (int n1 = 0; n1 < neuronsCountInLayer[l] - biosesInLayerCount; n1++)
		{
			if (l == (layersCount - 1))
				int point = 0;
			float net = 0;
			for (int n0 = 0; n0 < neuronsCountInLayer[l - 1]; n0++)
				net += neurons[l - 1][n0] * weights[l][n1][n0];

			neurons[l][n1] = sigmoid(net);
		}
}

float Perceptron::backPropagation(float* t, float a, float k)
{
	float error = updateErrors(t);

	for (int l = 1; l < layersCount; l++)
		for (int n0 = 0; n0 < neuronsCountInLayer[l - 1]; n0++) //Поменять с нижним циклом
			for (int n1 = 0; n1 < neuronsCountInLayer[l] - biosesInLayerCount; n1++)
			{
				float dw = (neuronsE[l][n1] * neurons[l - 1][n0]) * k + oldDiff[l][n1][n0] * a;
				weights[l][n1][n0] -= dw;
				oldDiff[l][n1][n0] = dw;
			}
	return error;
}

void Perceptron::use(float* inputData, float* outputData)
{
	float test = neurons[layersCount - 1][0];
	float inputData0 = inputData[0];
	float inputData1 = inputData[1];
	frontPropagation(inputData);
	test = neurons[layersCount - 1][0];

	for (int n = 0; n < neuronsCountInLayer[layersCount - 1] - biosesInLayerCount; n++)
		outputData[n] = neurons[layersCount - 1][n];
}

float Perceptron::getError(float* t)
{
	float error = 0;
	for (int n = 0; n < neuronsCountInLayer[layersCount - 1] - biosesInLayerCount; n++)
		error += pow((t[n] - neurons[layersCount - 1][n]), 2) / (float)neuronsCountInLayer[layersCount - 1];
	return error;
}

float Perceptron::getTotalError(Sample* samples, int samplesCount)
{
	float totalError = 0;

	for (int i = 0; i < samplesCount; i++)
	{
		frontPropagation(samples[i].inputData);
		totalError += getError(samples[i].outputData) / (float)samplesCount;
	}

	return totalError;
}



