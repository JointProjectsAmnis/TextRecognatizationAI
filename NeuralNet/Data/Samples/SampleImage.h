#pragma once
#include "ISample.h"
#include "../Image.h"
#include "../ImageData.h"
class SampleImage : public ISample
{
public:
	Image* imageInput = nullptr;
	Image* imageOutput = nullptr;

	SampleImage();
	SampleImage(Image* imageInput, Image* imageOutput);
	~SampleImage();

	virtual void realese() override;

	virtual double* getInputData(int id) override;
	virtual double* getOutputData(int id) override;

	static SampleImage** createSamples(ImageData* inputData, ImageData* outputData);
	//static SampleData* createSampleData(char* pathInput, char* pathOutput);

};

