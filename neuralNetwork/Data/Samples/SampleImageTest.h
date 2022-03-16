#pragma once

#include "../../Decl.h"
#include "ISample.h"
#include "../Image.h"
#include "../ImageData.h"
class SampleImageTest : public ISample
{
public:
	Image* imageInput = nullptr;
	Image* imageOutput = nullptr;

	DECL SampleImageTest();
	DECL SampleImageTest(Image* imageInput, Image* imageOutput);
	DECL ~SampleImageTest();

	DECL virtual void realese() override;

	DECL virtual double* getInputData(int id) override;
	DECL virtual double* getOutputData(int id) override;

	DECL static SampleImageTest** createSamples(ImageData* inputData, ImageData* outputData);
	//static SampleData* createSampleData(char* pathInput, char* pathOutput);

};

