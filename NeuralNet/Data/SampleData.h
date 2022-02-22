#pragma once
#include "Samples/SampleImage.h"
#include "ImageData.h"

class SampleData
{
public:
	
	int countSamples = 0;
	ISample** sample = nullptr;

	SampleData();
	SampleData(ISample** sample, int countSamples);
	SampleData(char* pathInput, char* pathOutput, int channel = -1);

	float* getInputData(int id);
	float* getOutputData(int id);

	//SampleImage* getSampleImage(int id);
	SampleImage* getSampleImage(int id);

	//Удаляет указатель sample, но не данные в нём
	void realesePointer();
	~SampleData();
};

