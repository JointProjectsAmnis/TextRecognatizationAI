#include "SampleData.h"

SampleData::SampleData()
{
	countSamples = 0;
	sample = nullptr;
}

SampleData::SampleData(ISample** sample, int countSamples)
{
	this->sample = sample;
	this->countSamples = countSamples;
}

SampleData::SampleData(char* pathInput, char* pathOutput, int channel)
{
	ImageData* inputData, *outputData;
	int countSamples;

	inputData = new ImageData(pathInput, channel);
	outputData = new ImageData(pathOutput, channel);

	ISample** sample = (ISample**)SampleImage::createSamples(inputData, outputData);
	countSamples = inputData->countImages > outputData->countImages ? outputData->countImages : inputData->countImages;
	this->sample = sample;
	this->countSamples = countSamples;

	delete inputData->image;
	delete outputData->image;
}

float* SampleData::getInputData(int id)
{
	return sample[id]->getInputData(id);
}

float* SampleData::getOutputData(int id)
{
	return sample[id]->getOutputData(id);
}

SampleImage* SampleData::getSampleImage(int id)
{
	return (SampleImage*)sample[id];
}

void SampleData::realesePointer()
{
	delete sample;
}

SampleData::~SampleData()
{
	for (int x = 0; x < countSamples; x++)
		sample[x]->realese();
	delete sample;
}
