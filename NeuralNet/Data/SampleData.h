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

	double* getInputData(int id);
	double* getOutputData(int id);

	//SampleImage* getSampleImage(int id);
	SampleImage* getSampleImage(int id);

	//??????? ????????? sample, ?? ?? ?????? ? ???
	void realesePointer();
	~SampleData();
};

