//#include <iostream>
//#include "Data/ImageData.h"
#include <soil.h>
#include <windows.h>

//#include "PathManager.h"
//#include "Data/ImageData.h"
//#include "Data/SampleData.h"
//#include "Data/Samples/SampleImage.h"
//#include "NeuralNet/NeuralNetDebug.cuh"
//#include "Data/ImageData.h"
#include "DeconvNeuralNetwork.h"
#include "ConvNeuralNetwork.h"


//INeuralNet* getNeuralNet(int sizeX, int sizeY, int channel)
//{
//	return new NeuralNetDebug(sizeX, sizeY, channel);
//}
//
//struct Sample
//{
//	double* inputData;
//	double** teatherData;
//};


void chToFl(unsigned char* chImage, double*& outFImage, int sizeX, int sizeY, int channel)
{
	if (outFImage == nullptr)
		outFImage = new double[sizeX * sizeY * channel];
	for (int x = 0; x < sizeX * sizeY * channel; x++)
		outFImage[x] = chImage[x] / 255.0f;
}

void deconv()
{
	setlocale(0, "ru");

	DeconvNeuralNetwork::DevconvNeuralNetDesc netDesc{};
	netDesc.branching = new int[] { 1, 0, 2};
	netDesc.defaultKernelOrigin = { 20, 20 };
	netDesc.defaultKernelSize = 40;
	netDesc.defaultUnpoolingSize = { 2, 2 };
	DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork({21, 21}, 3, netDesc);
	NetMatrix* lastMatrix = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0];
	neuralNetwork.setAllWeightsRandom(95, -1, 1, 1000);


	const char* pathInput = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Input\\image21x21.png";
	const char* pathTeather = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Teather\\*";
	const char* pathOutput = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Output\\OutputImage.bmp";
	int width, height, channels = 1;
	unsigned char* byteImage = SOIL_load_image(pathInput, &width, &height, &channels, 1);

	double* image = nullptr;
	chToFl(byteImage, image, width, height, 1);

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

void printNeuralNet(ConvNeuralNetwork& net)
{
	for (int l = net.layersCount - 1; l >= 0; l--)
	{
		for (int y = 0; y < net.matrices[l][0]->matrixSizeY; y++)
		{
			for (int m = 0; m < net.matricesCount[l]; m++)
			{
				for (int x = 0; x < net.matrices[l][m]->matrixSizeX; x++)
					std::cout << net.matrices[l][m]->matrix[x][y] << " ";
				std::cout << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

void printMatrix(NetMatrix &matrix)
{
	for (int y = 0; y < matrix.matrixSizeY; y++)
	{
		for (int x = 0; x < matrix.matrixSizeX; x++)
		{
			std::cout << matrix.matrix[x][y] << " ";
		}
		std::cout << std::endl;
	}
}

void printKernel(NetMatrix* matrix)
{
	for (int y = 0; y < matrix->kernelSize; y++)
	{
		for (int x = 0; x < matrix->kernelSize; x++)
		{
			std::cout << matrix->kernel[x][y] << " ";
		}
		std::cout << std::endl;
	}
}

void main()
{
	const char* pathInput = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Input\\image.png";
	const char* pathTeather = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Teather\\*";
	const char* pathOutput = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Output\\OutputImage.bmp";
	int width, height, channels = 1;
	unsigned char* byteImage = SOIL_load_image(pathInput, &width, &height, &channels, 1);


	double** inputData;
	ConvNeuralNetwork::ConvNeuralNetDesc netDesc{};

	int layersCount = 5;
	netDesc.branching = new int[] { 3, 0, 2, 0, 1 };
	//netDesc.branching = new int[layersCount] { 1, 1 };
	netDesc.defaultKernelOrigin = { 10, 10 };
	netDesc.defaultKernelSize = 20;
	netDesc.defaultPoolingSize = { 2, 2 };
	ConvNeuralNetwork net({ width, height }, layersCount, netDesc);
	net.setAllWeightsRandom(293, -1, 1, 1000);

	inputData = new double* [net.matrices[0][0]->matrixSizeX];
	for (int x = 0; x < net.matrices[0][0]->matrixSizeX; x++)
	{
		inputData[x] = new double[net.matrices[0][0]->matrixSizeY];
		for (int y = 0; y < net.matrices[0][0]->matrixSizeY; y++)
			inputData[x][y] = (rand() % 10) / 10.0;
	}

	double* image = nullptr;
	chToFl(byteImage, image, width, height, 1);

	double** intputMatrix = new double* [width];
	for (int x = 0; x < width; x++)
	{
		intputMatrix[x] = new double[height];
		for (int y = 0; y < height; y++)
		{
			intputMatrix[x][y] = image[x + y * width];
		}
	}

	int lastNeuronsCount = 0;
	for (int m = 0; m < net.matricesCount[net.layersCount - 1]; m++)
		lastNeuronsCount += net.matrices[net.layersCount - 1][m]->matrixSizeX * net.matrices[net.layersCount - 1][m]->matrixSizeY;

	double* outputData = new double[lastNeuronsCount];
	for (int i = 0; i < lastNeuronsCount; i++)
		outputData[i] = (rand() % 1000) / 1000.0;

	int countGeneration = 1000000;
	for (int g = 0; g < countGeneration; g++)
	{
		net.forwardPropagation(intputMatrix, net.matrices[0][0]->matrixSizeX * net.matrices[0][0]->matrixSizeY * sizeof(double));
		net.backPropagation(outputData, lastNeuronsCount * sizeof(double), 0.001f, 0.3f);

		if (g % 100 == 0)
			std::cout << net.getError(outputData, lastNeuronsCount * sizeof(double)) << std::endl;
	}
}
