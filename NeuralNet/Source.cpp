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

	netDesc.branching = new int[] { 1, 1, 2, 1, 3 };
	netDesc.defaultKernelOrigin = { 1, 1 };
	netDesc.defaultKernelSize = 3;
	netDesc.inputMatricesCount = 6;
	netDesc.intputMatricesSize = {140, 70};
	netDesc.layersCount = 5;
	netDesc.unpoolingLayers = new int[] {0, 1, 0, 1, 0};
	DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork(netDesc);
	neuralNetwork.setAllWeightsRandom(77, -1, 1, 1000);

	//for (int m = 0; m < neuralNetwork.matricesCount[0]; m++)
	//{
	//	neuralNetwork.matrices[0][m]->kernel[0][0] = 1;
	//	neuralNetwork.matrices[0][m]->kernel[1][0] = 2;
	//	neuralNetwork.matrices[0][m]->kernel[2][0] = 3;

	//	neuralNetwork.matrices[0][m]->kernel[0][1] = 0;
	//	neuralNetwork.matrices[0][m]->kernel[1][1] = 2;
	//	neuralNetwork.matrices[0][m]->kernel[2][1] = 1;

	//	neuralNetwork.matrices[0][m]->kernel[0][2] = 1;
	//	neuralNetwork.matrices[0][m]->kernel[1][2] = 0;
	//	neuralNetwork.matrices[0][m]->kernel[2][2] = 1;
	//}


	int inputElementsCount = netDesc.inputMatricesCount * netDesc.intputMatricesSize.x * netDesc.intputMatricesSize.y;
	//double* inputData = new double[inputElementsCount] {0, 0.1, 0.2, 0.3, 0.3, 0.2, 0.1, 0};
	double* inputData = new double[inputElementsCount];
	for (int i = 0; i < inputElementsCount; i++)
		inputData[i] = ((rand() % 1000) / (double)1000) * 2;

	neuralNetwork.forwardPropagation(inputData, inputElementsCount * sizeof(double));

	//for (int l = 0; l < neuralNetwork.layersCount; l++)
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


	NetMatrix &matrix2 = *(neuralNetwork.matrices[neuralNetwork.layersCount - 1][0]);

	unsigned char* outImage = new unsigned char[matrix2.matrixSizeX * matrix2.matrixSizeY];
	int id = 0;
	for (int y = 0; y < matrix2.matrixSizeY; y++)
		for(int x = 0; x < matrix2.matrixSizeX; x++)
		{
			outImage[id] = matrix2.matrix[x][y] * 255;
			id++;
		}

	SOIL_save_image("E:\\Университет\\Английский язык\\Проект (нейросеть)\\TextRecognatizationAI\\Images\\image.bmp", SOIL_SAVE_TYPE_BMP, matrix2.matrixSizeX, matrix2.matrixSizeY, 1, outImage);


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