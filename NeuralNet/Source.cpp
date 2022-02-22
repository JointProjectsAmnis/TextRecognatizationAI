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

	NetMatrix matrix0(2, 2);
	NetMatrix matrix1(8, 8);

	matrix0.matrix[0][0] = 1;
	matrix0.matrix[1][0] = 2;

	matrix0.matrix[0][1] = 3;
	matrix0.matrix[1][1] = 4;

	matrix0.unpool(&matrix1, 4, 4);

	for (int y = 0; y < matrix1.matrixSizeX; y++)
	{
		for (int x = 0; x < matrix1.matrixSizeY; x++)
		{
			std::cout << matrix1.matrix[x][y] << " ";
		}
		std::cout << std::endl;
	}



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