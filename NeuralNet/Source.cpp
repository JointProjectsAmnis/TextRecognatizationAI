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

	DeconvNeuralNetwork::DevconvNeuralNetDesc netDesc{};
	netDesc.branching = new int[] { 1, 1, 1, 2, 1, 2 };
	netDesc.defaultKernelOrigin = { 1, 1 };
	netDesc.defaultKernelSize = 40;
	netDesc.inputMatricesCount = 4;
	netDesc.intputMatricesSize = { 10, 10 };
	netDesc.layersCount = 6;
	netDesc.defaultUnpoolingSize = {2, 2};
	netDesc.unpoolingLayers = new int[] {0, 1, 1, 0, 1, 0};
	DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork(netDesc);
	NetMatrix* lastMatrix = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0];
	neuralNetwork.setAllWeightsRandom(674, -1, 1, 1000);


	const char* pathInput = "E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\Input\\image80x80.png";
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
	netDesc.intputMatricesSize.x;
	double* inputData = new double[neuralNetwork.intputMatricesSize.x * neuralNetwork.intputMatricesSize.y * netDesc.inputMatricesCount];
	for (int x = 0; x < neuralNetwork.intputMatricesSize.x * neuralNetwork.intputMatricesSize.y * netDesc.inputMatricesCount; x++)
		inputData[x] = (rand() % 1000) / 1000.0f;


	int countGeneration = 10000;
	for (int g = 0; g < countGeneration; g++)
	{
		neuralNetwork.forwardPropagation(inputData, neuralNetwork.intputMatricesSize.x * neuralNetwork.intputMatricesSize.y * netDesc.inputMatricesCount * sizeof(double));
		neuralNetwork.backPropagation(matrix, width * height * sizeof(double), 0.001f, 0.3f);

		if (g % 100 == 0)
		{
			static unsigned char* outputImage = new unsigned char[lastMatrix->matrixSizeX * lastMatrix->matrixSizeY];
			int tid = 0;
			for (int y = 0; y < lastMatrix->matrixSizeX; y++)
				for (int x = 0; x < lastMatrix->matrixSizeY; x++)
				{
					outputImage[tid] = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrix[x][y] * 255;
					tid++;
				}

			SOIL_save_image(pathOutput, SOIL_SAVE_TYPE_BMP, lastMatrix->matrixSizeX, lastMatrix->matrixSizeY, 1, outputImage);
		}
	}

	///////////////////////////////////////////////////////////
	//double** inputMatrix = new double*[width];
	//double** outputMatrix = new double*[width];
	//for (int x = 0; x < width; x++)
	//{
	//	inputMatrix[x] = new double[height];
	//	outputMatrix[x] = new double[height];
	//	for (int y = 0; y < height; y++)
	//	{
	//		inputMatrix[x][y] = image[x + y * width];
	//		outputMatrix[x][y] = 0;
	//	}
	//}
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	//int sizeKernel = 5;
	//double** kernel = new double*[sizeKernel];
	//for (int x = 0; x < sizeKernel; x++)
	//{
	//	kernel[x] = new double[sizeKernel];
	//	for (int y = 0; y < sizeKernel; y++)
	//		kernel[x][y] = (rand() % 1000) / 1000.0;
	//}
	///////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////

	//for(int x = 0; x < width; x++)
	//	for (int y = 0; y < height; y++)
	//	{
	//		for(int kx = 0; kx < sizeKernel; kx++)
	//			for (int ky = 0; ky < sizeKernel; ky++)
	//			{
	//				int posInMatrixX = x - (sizeKernel / 2 + 1) + kx;
	//				int posInMatrixY = y - (sizeKernel / 2 + 1) + ky;
	//				outputMatrix[posInMatrixX][posInMatrixY] += 
	//			}
	//	}

	///////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//int countImages;
	//char** names = PathManager::getFolderNames((char*)pathTeather, &countImages);

	//int sizeImageX, sizeImageY, channels;

	//Sample* sample = new Sample[countImages];
	//for (int x = 0; x < countImages; x++)
	//{
	//	double* input = new double[neuralNetwork.matricesCount[0]];
	//	for (int y = 0; y < neuralNetwork.matricesCount[0]; y++)
	//		input[y] = (rand() % 1000) / 1000.0f;

	//	unsigned char* byteImage = SOIL_load_image(names[x], &sizeImageX, &sizeImageY, &channels, 1);
	//	channels = 1;

	//	static double* image = nullptr;
	//	Image::chToFl(byteImage, image, sizeImageX, sizeImageY, 1);

	//	delete byteImage;

	//	
	//	double** teather = new double*[lastMatrix->matrixSizeX];
	//	for (int mx = 0; mx < lastMatrix->matrixSizeX; mx++)
	//	{
	//		teather[mx] = new double[lastMatrix->matrixSizeY];
	//		for (int my = 0; my < lastMatrix->matrixSizeY; my++)
	//			teather[mx][my] = mx < sizeImageX && my < sizeImageY ? image[mx + my * sizeImageX] : 0;
	//	}

	//	sample[x] = {input, teather };
	//}

	//int countGeneration = 1000;
	//for (int g = 0; g < countGeneration; g++)
	//{
	//	int idSample = 3;

	//	neuralNetwork.forwardPropagation(sample[idSample].inputData, sizeof(double) * neuralNetwork.matricesCount[0]);
	//	neuralNetwork.backPropagation(sample[idSample].teatherData, lastMatrix->matrixSizeX * lastMatrix->matrixSizeY * sizeof(double), 0.01f, 0.3f);

	//	if (g % 1 == 0)
	//	{
	//		static unsigned char* outputImage = new unsigned char[lastMatrix->matrixSizeX * lastMatrix->matrixSizeY];
	//		int tid = 0;
	//		for (int y = 0; y < lastMatrix->matrixSizeX; y++)
	//			for (int x = 0; x < lastMatrix->matrixSizeY; x++)
	//			{
	//				outputImage[tid] = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrix[x][y] * 255;
	//				tid++;
	//			}

	//		SOIL_save_image(pathOutput, SOIL_SAVE_TYPE_BMP, lastMatrix->matrixSizeX, lastMatrix->matrixSizeY, 1, outputImage);
	//	}
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

















	//double** output = new double* [neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrixSizeX];
	//for (int i = 0; i < neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrixSizeX; i++)
	//	output[i] = new double[neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrixSizeY];

	////output[0][0] = 1;
	////output[1][0] = 0.5;
	////output[0][1] = 0;
	////output[1][1] = 0.3;

	//int width = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrixSizeX;
	//int height = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrixSizeY;

	////neuralNetwork.backPropagation(output, width * height * sizeof(double));

	////for (int y = 0; y < neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrixSizeY; y++)
	////{
	////	for (int x = 0; x < neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]->matrixSizeX; x++)
	////	{
	////		std::cout << neuralNetwork.errorMatrices[neuralNetwork.layersCount - 1][0]->matrix[x][y] << " ";
	////	}
	////	std::cout << std::endl;
	////}



	////neuralNetwork.setAllWeightsRandom(77, -1, 1, 1000);
	////neuralNetwork.getParentMatrix(3, 2);


	////int inputElementsCount = netDesc.inputMatricesCount * netDesc.intputMatricesSize.x * netDesc.intputMatricesSize.y;
	////double* inputData = new double[inputElementsCount];
	////for (int i = 0; i < inputElementsCount; i++)
	////	inputData[i] = ((rand() % 1000) / (double)1000) * 2;

	////neuralNetwork.forwardPropagation(inputData, inputElementsCount * sizeof(double));


	//// Output errors

	////for (int l = neuralNetwork.layersCount - 1; l >= 0; l--)
	////{
	////	for (int y = 0; y < neuralNetwork.matrices[l][0]->matrixSizeY; y++)
	////	{
	////		for (int m = 0; m < neuralNetwork.matricesCount[l]; m++)
	////		{
	////			for (int x = 0; x < neuralNetwork.matrices[l][m]->matrixSizeX; x++)
	////				std::cout << neuralNetwork.errorMatrices[l][m]->matrix[x][y] << " ";
	////			std::cout << " ";
	////		}
	////		std::cout << std::endl;
	////	}
	////	std::cout << std::endl << std::endl;
	////}

	//// Output matrices

	//for (int l = neuralNetwork.layersCount - 1; l >= 0; l--)
	//{
	//	for (int y = 0; y < neuralNetwork.matrices[l][0]->matrixSizeY; y++)
	//	{
	//		for (int m = 0; m < neuralNetwork.matricesCount[l]; m++)
	//		{
	//			for (int x = 0; x < neuralNetwork.matrices[l][m]->matrixSizeX; x++)
	//				std::cout << neuralNetwork.matrices[l][m]->matrix[x][y] << " ";
	//			std::cout << " ";
	//		}
	//		std::cout << std::endl;
	//	}
	//	std::cout << std::endl << std::endl;
	//}


	//NetMatrix &matrix2 = *(neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]);

	//unsigned char* outImage = new unsigned char[matrix2.matrixSizeX * matrix2.matrixSizeY];
	//int id = 0;
	//for (int y = 0; y < matrix2.matrixSizeY; y++)
	//	for(int x = 0; x < matrix2.matrixSizeX; x++)
	//	{
	//		outImage[id] = matrix2.matrix[x][y] * 255;
	//		id++;
	//	}

	//SOIL_save_image("E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\image.bmp", SOIL_SAVE_TYPE_BMP, matrix2.matrixSizeX, matrix2.matrixSizeY, 1, outImage);


	////char pathInput[2000];
	////char pathTeather[2000];
	////char pathOutput[2000];
	////char path[2000];
	////PathManager::getFolderWithExe(pathInput);
	////PathManager::getFolderWithExe(pathTeather);
	////PathManager::getFolderWithExe(pathOutput);

	////strcat(pathInput, "\\NumberData\\Input\\*");
	////strcat(pathTeather, "\\NumberData\\Teather\\*");
	////strcat(pathOutput, "\\Results\\*");

	////SampleData sampleData = SampleData(pathInput, pathTeather, 3);
	////Image* image = sampleData.getSampleImage(0)->imageInput;
	////INeuralNet* net = getNeuralNet(image->sizeX, image->sizeY, image->channel);

	////image = nullptr;
	////int countGeneration = 1000;
	////for (int x = 0; x < countGeneration; x++)
	////{
	////	int r = rand() % sampleData.countSamples;
	////	net->learnOne(sampleData.sample[r]);
	////	
	////	if (x % 1000 == 0)
	////	{
	////		net->getImage(image);
	////		//image->saveImage(PathManager::addNameInstance(pathOutput, path, 1, "\\OutputImage.bmp"));
	////	}
	////}
	//
}

//setlocale(0, "ru");
//char pathInput[2000];
//char pathTeather[2000];
//char pathOutput[2000];
//char pathTest[2000];
//PathManager::getFolderWithExe(pathInput);
//PathManager::getFolderWithExe(pathTeather);
//PathManager::getFolderWithExe(pathOutput);
//PathManager::getFolderWithExe(pathTest);
//
//strcat(pathInput, "\\Images\\InputImages\\*");
//strcat(pathTeather, "\\Images\\OutputImages\\*");
//strcat(pathOutput, "\\OutImages\\*");
//strcat(pathTest, "\\TestOutput\\FromDevice.bmp");
//
//SampleData* sampleData = new SampleData(pathInput, pathTeather, 3);
//Image* tempImage;
//
//tempImage = new Image(sampleData->getOutputData(1), 1920, 1080, 3, false);
//tempImage->saveImage(pathTest);