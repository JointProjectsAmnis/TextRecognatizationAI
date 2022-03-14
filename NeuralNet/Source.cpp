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

//void main1()
//{
//	setlocale(0, "ru");
//
//
//	//DeconvNeuralNetwork::PartialDevconvNeuralNetDesc netDesc{};
//	//netDesc.branching = new int[] { 1, 0, 2, 1 };
//	//netDesc.defaultKernelOrigin = { 20, 20 };
//	//netDesc.defaultKernelSize = 40;
//	//netDesc.defaultUnpoolingSize = { 2, 2 };
//	//DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork({ 20, 20 }, 4, netDesc);
//
//	DeconvNeuralNetwork::DevconvNeuralNetDesc netDesc{};
//	netDesc.branching = new int[] { 1, 1, 2, 1};
//	netDesc.defaultKernelOrigin = { 20, 20 };
//	netDesc.defaultKernelSize = 40;
//	netDesc.inputMatricesCount = 2;
//	netDesc.intputMatricesSize = { 10, 10 };
//	netDesc.layersCount = 4;
//	netDesc.defaultUnpoolingSize = {2, 2};
//	netDesc.unpoolingLayers = new int[] {0, 1, 0, 0};
//
//	DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork(netDesc);
//	NetMatrix* lastMatrix = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0];
//	neuralNetwork.setAllWeightsRandom(674, -1, 1, 1000);
//
//
//	const char* pathInput = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Input\\image20x20.png";
//	const char* pathTeather = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Teather\\*";
//	const char* pathOutput = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Output\\OutputImage.bmp";
//	int width, height, channels = 1;
//	unsigned char* byteImage = SOIL_load_image(pathInput, &width, &height, &channels, 1);
//
//	double* image = nullptr;
//	Image::chToFl(byteImage, image, width, height, 1);
//
//	double** matrix = new double* [width];
//	for (int x = 0; x < width; x++)
//	{
//		matrix[x] = new double[height];
//		for (int y = 0; y < height; y++)
//		{
//			matrix[x][y] = image[x + y * width];
//		}
//	}
//	//netDesc.intputMatricesSize.x;
//
//	double* inputData = new double[neuralNetwork.intputMatricesSize.x * neuralNetwork.intputMatricesSize.y * netDesc.inputMatricesCount];
//	for (int x = 0; x < neuralNetwork.intputMatricesSize.x * neuralNetwork.intputMatricesSize.y * netDesc.inputMatricesCount; x++)
//		inputData[x] = (rand() % 1000) / 1000.0f;
//
//
//	int countGeneration = 1000;
//	for (int g = 0; g < countGeneration; g++)
//	{
//		neuralNetwork.forwardPropagation(inputData, neuralNetwork.intputMatricesSize.x * neuralNetwork.intputMatricesSize.y * netDesc.inputMatricesCount * sizeof(double));
//		//neuralNetwork.forwardPropagationWithoutUnPoolingLayers(inputData, inputDataCount * sizeof(double));
//		neuralNetwork.backPropagation(matrix, width * height * sizeof(double), 0.001f, 0.3f);
//		//neuralNetwork.backPropagationWithoutUnPoolingLayers(matrix, width * height * sizeof(double), 0.001f, 0.3f);
//
//		if (g % 100 == 0)
//		{
//			static unsigned char* outputImage = new unsigned char[lastMatrix->matrixSizeX * lastMatrix->matrixSizeY];
//			int tid = 0;
//			for (int y = 0; y < lastMatrix->matrixSizeX; y++)
//				for (int x = 0; x < lastMatrix->matrixSizeY; x++)
//				{
//					outputImage[tid] = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrix[x][y] * 255;
//					tid++;
//				}
//
//			SOIL_save_image(pathOutput, SOIL_SAVE_TYPE_BMP, lastMatrix->matrixSizeX, lastMatrix->matrixSizeY, 1, outputImage);
//		}
//	}
//}


void main()
{
	setlocale(0, "ru");

	DeconvNeuralNetwork::PartialDevconvNeuralNetDesc netDesc{};
	//netDesc.branching = new int[] { 1, 0, 2, 0, 3};
	netDesc.branching = new int[] { 1, 0, 1};
	//netDesc.branching = new int[] { 1, 1, 1 };
	netDesc.defaultKernelOrigin = { 20, 20 };
	netDesc.defaultKernelSize = 40;
	netDesc.defaultUnpoolingSize = { 2, 2 };
	DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork({21, 21}, 3, netDesc);
	//DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork({20, 20}, 3, netDesc);
	NetMatrix* lastMatrix = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0];
	//neuralNetwork.setAllWeightsRandom(15, -1, 1, 1000);
	neuralNetwork.setAllWeightsRandom(95, -1, 1, 1000);


	const char* pathInput = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Input\\image21x21.png";
	const char* pathTeather = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Teather\\*";
	const char* pathOutput = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Output\\OutputImage.bmp";
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

	//unsigned char* testByte = new unsigned char [width * height * channels];

	//for (int y = 0; y < height; y++)
	//	for (int x = 0; x < width; x++)
	//	{
	//		testByte[x + y * width] = matrix[x][y] * 255.0f;
	//	}

	//SOIL_save_image(pathOutput, SOIL_SAVE_TYPE_BMP, lastMatrix->matrixSizeX, lastMatrix->matrixSizeY, 1, testByte);




	//netDesc.intputMatricesSize.x;
	
	//double* inputData = new double[neuralNetwork.intputMatricesSize.x * neuralNetwork.intputMatricesSize.y * netDesc.inputMatricesCount];
	//for (int x = 0; x < neuralNetwork.intputMatricesSize.x * neuralNetwork.intputMatricesSize.y * netDesc.inputMatricesCount; x++)
	//	inputData[x] = (rand() % 1000) / 1000.0f;

	int inputDataCount = neuralNetwork.matrices[0][0]->matrixSizeX * neuralNetwork.matrices[0][0]->matrixSizeY * neuralNetwork.matricesCount[0];
	double* inputData = new double[inputDataCount];
	for (int x = 0; x < inputDataCount; x++)
		inputData[x] = (rand() % 1000) / 1000.0f;




	int countGeneration = 10000;
	for (int g = 0; g < countGeneration; g++)
	{
		//neuralNetwork.forwardPropagation(inputData, neuralNetwork.intputMatricesSize.x * neuralNetwork.intputMatricesSize.y * netDesc.inputMatricesCount * sizeof(double));
		neuralNetwork.forwardPropagationWithoutUnPoolingLayers(inputData, inputDataCount * sizeof(double));
		//neuralNetwork.backPropagation(matrix, width * height * sizeof(double), 0.001f, 0.3f);
		neuralNetwork.backPropagationWithoutUnPoolingLayers(matrix, width * height * sizeof(double), 0.001f, 0.3f);

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
