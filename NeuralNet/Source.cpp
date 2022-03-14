#include <iostream>
#include "Data/ImageData.h"
#include <soil.h>
#include <windows.h>

#include "PathManager.h"
#include "Data/ImageData.h"
#include "Data/SampleData.h"
#include "Data/Samples/SampleImage.h"
#include "NeuralNet/NeuralNetDebug.cuh"
#include "DeconvNeuralNetwork.h"
#include "Data/ImageData.h"


INeuralNet* getNeuralNet(int sizeX, int sizeY, int channel)
{
	return new NeuralNetDebug(sizeX, sizeY, channel);
}

struct Sample
{
	double* inputData;
	double** teatherData;
};

void main()
{
	setlocale(0, "ru");

	DeconvNeuralNetwork::PartialDevconvNeuralNetDesc netDesc{};
	netDesc.branching = new int[] { 1, 0, 2};
	netDesc.defaultKernelOrigin = { 20, 20 };
	netDesc.defaultKernelSize = 40;
	netDesc.defaultUnpoolingSize = { 2, 2 };
	DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork({21, 21}, 3, netDesc);
	NetMatrix* lastMatrix = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0];
	neuralNetwork.setAllWeightsRandom(95, -1, 1, 1000);


	const char* pathInput = "E:\\�����������\\���������� ����\\������ (���������)\\TextRecognatizationAI\\Images\\Input\\image21x21.png";
	const char* pathTeather = "E:\\�����������\\���������� ����\\������ (���������)\\TextRecognatizationAI\\Images\\Teather\\*";
	const char* pathOutput = "E:\\�����������\\���������� ����\\������ (���������)\\TextRecognatizationAI\\Images\\Output\\OutputImage.bmp";
	int width, height, channels = 1;
	unsigned char* byteImage = SOIL_load_image(pathInput, &width, &height, &channels, 1);

	double* image = nullptr;
	Image::chToFl(byteImage, image, width, height, 1);

	double** matrix = new double* [width];
	for (int x = 0; x < width; x++)
	{
		matrix[x] = new double[height];
		for (int y = 0; y < height; y++)
		{
			matrix[x][y] = image[x + y * width];
		}
	}

	int inputDataCount = neuralNetwork.matrices[0][0]->matrixSizeX * neuralNetwork.matrices[0][0]->matrixSizeY * neuralNetwork.matricesCount[0];
	double* inputData = new double[inputDataCount];
	for (int x = 0; x < inputDataCount; x++)
		inputData[x] = (rand() % 1000) / 1000.0f;

	int countGeneration = 10000;
	for (int g = 0; g < countGeneration; g++)
	{
		neuralNetwork.forwardPropagation(inputData, inputDataCount * sizeof(double));
		neuralNetwork.backPropagation(matrix, width * height * sizeof(double), 0.001f, 0.3f);

		if (g % 100 == 0)
		{
			static unsigned char* outputImage = new unsigned char[lastMatrix->matrixSizeX * lastMatrix->matrixSizeY];
			int tid = 0;
			for (int y = 0; y < lastMatrix->matrixSizeY; y++)
				for (int x = 0; x < lastMatrix->matrixSizeX; x++)
				{
					outputImage[tid] = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrix[x][y] * 255;
					tid++;
				}

			std::cout << neuralNetwork.getError(matrix, width * height * sizeof(double)) << std::endl;

			SOIL_save_image(pathOutput, SOIL_SAVE_TYPE_BMP, lastMatrix->matrixSizeX, lastMatrix->matrixSizeY, 1, outputImage);
		}
	}
}
