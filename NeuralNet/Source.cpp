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

INeuralNet* getNeuralNet(int sizeX, int sizeY, int channel)
{
	return new NeuralNetDebug(sizeX, sizeY, channel);
}

void main()
{
	setlocale(0, "ru");

	DeconvNeuralNetwork::DevconvNeuralNetDesc netDesc{};

	netDesc.branching = new int[] { 1, 2, 1, 3, 1 };
	netDesc.defaultKernelOrigin = { 1, 1 };
	netDesc.defaultKernelSize = 3;
	netDesc.inputMatricesCount = 6;
	netDesc.intputMatricesSize = {2, 2};
	netDesc.layersCount = 5;
	netDesc.unpoolingLayers = new int[] {0, 1, 0, 1, 0};
	DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork(netDesc);

	//unsigned char* outImage = new unsigned char[matrix2.matrixSizeX * matrix2.matrixSizeY];
	//int id = 0;
	//for (int y = 0; y < matrix2.matrixSizeY; y++)
	//	for(int x = 0; x < matrix2.matrixSizeX; x++)
	//	{
	//		outImage[id] = matrix2.matrix[x][y] * 255;
	//		id++;
	//	}

	//SOIL_save_image("E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\image.bmp", SOIL_SAVE_TYPE_BMP, matrix2.matrixSizeX, matrix2.matrixSizeY, 1, outImage);


	//char pathInput[2000];
	//char pathTeather[2000];
	//char pathOutput[2000];
	//char path[2000];
	//PathManager::getFolderWithExe(pathInput);
	//PathManager::getFolderWithExe(pathTeather);
	//PathManager::getFolderWithExe(pathOutput);

	//strcat(pathInput, "\\NumberData\\Input\\*");
	//strcat(pathTeather, "\\NumberData\\Teather\\*");
	//strcat(pathOutput, "\\Results\\*");

	//SampleData sampleData = SampleData(pathInput, pathTeather, 3);
	//Image* image = sampleData.getSampleImage(0)->imageInput;
	//INeuralNet* net = getNeuralNet(image->sizeX, image->sizeY, image->channel);

	//image = nullptr;
	//int countGeneration = 1000;
	//for (int x = 0; x < countGeneration; x++)
	//{
	//	int r = rand() % sampleData.countSamples;
	//	net->learnOne(sampleData.sample[r]);
	//	
	//	if (x % 1000 == 0)
	//	{
	//		net->getImage(image);
	//		//image->saveImage(PathManager::addNameInstance(pathOutput, path, 1, "\\OutputImage.bmp"));
	//	}
	//}
	
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