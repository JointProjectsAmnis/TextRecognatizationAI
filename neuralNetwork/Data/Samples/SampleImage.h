#pragma once
#include "../../Decl.h"
#include "ISample.h"
#include "../Image.h"
#include "../ImageData.h"
class SampleImage : public ISample
{
public:
	Image* imageInput = nullptr;
	Image* imageOutput = nullptr;

	DECL SampleImage();
	DECL SampleImage(Image* imageInput, Image* imageOutput);
	DECL ~SampleImage();

	DECL virtual void realese() override;

	DECL virtual double* getInputData(int id) override;
	DECL virtual double* getOutputData(int id) override;

	DECL static SampleImage** createSamples(ImageData* inputData, ImageData* outputData);
	//static SampleData* createSampleData(char* pathInput, char* pathOutput);

};

