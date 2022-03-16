#pragma once
#include "ISample.h"
#include "../Image.h"
#include "../ImageData.h"
class SampleImageTest : public ISample
{
public:
	Image* imageInput = nullptr;
	Image* imageOutput = nullptr;

	SampleImageTest();
	SampleImageTest(Image* imageInput, Image* imageOutput);
	~SampleImageTest();

	virtual void realese() override;

	virtual double* getInputData(int id) override;
	virtual double* getOutputData(int id) override;

	static SampleImageTest** createSamples(ImageData* inputData, ImageData* outputData);
	//static SampleData* createSampleData(char* pathInput, char* pathOutput);

};

