#pragma once
#include "../Decl.h"
#include "Samples/SampleImage.h"
#include "ImageData.h"

class SampleData
{
public:
	
	int countSamples = 0;
	ISample** sample = nullptr;

	DECL SampleData();
	DECL SampleData(ISample** sample, int countSamples);
	DECL SampleData(char* pathInput, char* pathOutput, int channel = -1);

	DECL double* getInputData(int id);
	DECL double* getOutputData(int id);

	//SampleImage* getSampleImage(int id);
	DECL SampleImage* getSampleImage(int id);

	//Удаляет указатель sample, но не данные в нём
	DECL void realesePointer();
	DECL ~SampleData();
};

