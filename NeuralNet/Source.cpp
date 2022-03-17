#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//Window application
#include "Window application/WindowInterfaceForNeuralNet.h"
#include "Window application/GraphicsContextImage.h"
#include "Window application/Content.h"

//Compute shader
#include "WindowsClasses/DirectX3D/Shaders/ShaderCompute.h"
#include "WindowsClasses/DirectX3D/UnorderedBuffer.h"


void mainGraphic()
{
	setlocale(LC_ALL, "ru");
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	Content::Load();

	WindowInterfaceForNeuralNet* windowInterface = new WindowInterfaceForNeuralNet(Content::brushBackground, Content::brushIdentity, Content::brushHot, Content::brushSelected, *Content::font);
	windowInterface->ShowWindowInterface(1);

	GraphicsContextImage* graphicsContextImage = new GraphicsContextImage(windowInterface);
	windowInterface->SetGraphics(graphicsContextImage);

	ShaderCompute shader = ShaderCompute(graphicsContextImage->graphics, L"WindowsClasses\\DirectX3D\\Shaders\\ShaderComputeTest\\ShaderComputeTest.hlsl");

	int sizeData = 1024;
	float* buffer = new float[sizeData];
	for (int x = 0; x < sizeData; x++)
		buffer[x] = 123;

	UnorderedBuffer outputBuffer = UnorderedBuffer(graphicsContextImage->graphics, sizeData, sizeof(float), buffer);
	UnorderedBuffer outputBufferResult = UnorderedBuffer(graphicsContextImage->graphics, sizeData, sizeof(float), D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_READ);

	outputBuffer.Bind(0);
	shader.BindShader();
	graphicsContextImage->graphics->Dispatch(32, 32, 1);

	outputBufferResult.CopyResource(outputBuffer.buffer);

	outputBufferResult.Map(D3D11_MAP_READ);
	outputBufferResult.ReadMem(buffer, sizeData * sizeof(float), 0);
	outputBufferResult.UnMap();

	for (int x = 0; x < sizeData; x++)
		std::cout << buffer[x] << std::endl;

	MSG msg = {};
	while (!windowInterface->windowMain->isClosed)
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		graphicsContextImage->Draw();
	}
}
































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
	DeconvNeuralNetwork neuralNetwork = DeconvNeuralNetwork({ 21, 21 }, 3, netDesc);
	NetMatrix* lastMatrix = neuralNetwork.matrices[neuralNetwork.layersCount - 1][0];
	neuralNetwork.setAllWeightsRandom(95, -1, 1, 1000);


	const char* pathInput = "E:\\�����������\\���������� ����\\������ (���������)\\TextRecognatizationAI\\Images\\Input\\image21x21.png";
	const char* pathTeather = "E:\\�����������\\���������� ����\\������ (���������)\\TextRecognatizationAI\\Images\\Teather\\*";
	const char* pathOutput = "E:\\�����������\\���������� ����\\������ (���������)\\TextRecognatizationAI\\Images\\Output\\OutputImage.bmp";
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

void printMatrix(NetMatrix* matrix)
{
	for (int y = 0; y < matrix->matrixSizeY; y++)
	{
		for (int x = 0; x < matrix->matrixSizeX; x++)
		{
			std::cout << matrix->matrix[x][y] << " ";
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

void conv()
{
	const char* pathInput = "..\\Images\\Input\\image.png";
	const char* pathTeather = "..\\Images\\Teather\\*";
	const char* pathOutput = "..\\Images\\Output\\OutputImage.bmp";
	int width, height, channels = 1;
	unsigned char* byteImage = SOIL_load_image(pathInput, &width, &height, &channels, 1);

	width = 10;
	height = 10;

	double** inputData;
	ConvNeuralNetwork::ConvNeuralNetDesc netDesc{};

	int layersCount = 5;
	netDesc.branching = new int[] { 3, 0, 2, 0, 1 };
	netDesc.defaultKernelOrigin = { 5, 5 };
	netDesc.defaultKernelSize = 10;
	netDesc.defaultPoolingSize = { 2, 2 };
	ConvNeuralNetwork net({ width, height }, layersCount, netDesc);
	net.setAllWeightsRandom(746, -1, 1, 1000);

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

	//intputMatrix[0][0] = 0;
	//intputMatrix[1][0] = 0.1;
	//intputMatrix[0][1] = 0.2;
	//intputMatrix[1][1] = 0.3;


	//net.matrices[1][0]->kernel[0][0] = 1;
	//net.matrices[1][0]->kernel[1][0] = 2;
	//net.matrices[1][0]->kernel[2][0] = 3;
	//net.matrices[1][0]->kernel[0][1] = 2;
	//net.matrices[1][0]->kernel[1][1] = 1;
	//net.matrices[1][0]->kernel[2][1] = 0;
	//net.matrices[1][0]->kernel[0][2] = 1;
	//net.matrices[1][0]->kernel[1][2] = 0;
	//net.matrices[1][0]->kernel[2][2] = 1;

	int lastNeuronsCount = 0;
	for (int m = 0; m < net.matricesCount[net.layersCount - 1]; m++)
		lastNeuronsCount += net.matrices[net.layersCount - 1][m]->matrixSizeX * net.matrices[net.layersCount - 1][m]->matrixSizeY;

	double* outputData = new double[lastNeuronsCount];
	for (int i = 0; i < lastNeuronsCount; i++)
		outputData[i] = (rand() % 1000) / 1000.0;
	//outputData[0] = 0.5;
	//outputData[1] = 0.6;
	//outputData[2] = 0;
	//outputData[3] = 0.1;
	// 
	//net.forwardPropagation(intputMatrix, width * height * sizeof(double));
	//printNeuralNet(net);
	//std::cout << std::endl << std::endl << std::endl;
	//net.backPropagation(outputData, lastNeuronsCount * sizeof(double), 0.01f, 0.3f);
	//printKernel(net.matrices[1][0]);

	int countGeneration = 10000000;
	for (int g = 0; g < countGeneration; g++)
	{
		net.forwardPropagation(intputMatrix, net.matrices[0][0]->matrixSizeX * net.matrices[0][0]->matrixSizeY * sizeof(double));
		net.backPropagation(outputData, lastNeuronsCount * sizeof(double), 0.01f, 0.3);

		if (g % 100 == 0)
			std::cout << net.getError(outputData, lastNeuronsCount * sizeof(double)) << std::endl;
	}
}

void main()
{

}
