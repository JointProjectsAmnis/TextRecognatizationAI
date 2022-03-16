#include "pch.h"
#include "SampleImage.h"

SampleImage::SampleImage()
{
}

SampleImage::SampleImage(Image* imageInput, Image* imageOutput)
{
    this->imageInput = imageInput;
    this->imageOutput = imageOutput;
}

SampleImage::~SampleImage()
{
    realese();
}

double* SampleImage::getInputData(int id)
{
    return imageInput->dev_image;
}

double* SampleImage::getOutputData(int id)
{
    return imageOutput->dev_image;
}

SampleImage** SampleImage::createSamples(ImageData* inputData, ImageData* outputData)
{
    int countSamples = inputData->countImages > outputData->countImages ? outputData->countImages : inputData->countImages;
    SampleImage** sample = new SampleImage*[countSamples];
    for (int x = 0; x < countSamples; x++)
        sample[x] = new SampleImage(inputData->image[x], outputData->image[x]);
    return sample;
}

//SampleData* SampleImage::createSampleData(char* pathInput, char* pathOutput)
//{
//    ImageData* inputData, *outputData;
//
//    inputData = new ImageData(pathInput);
//    outputData = new ImageData(pathOutput);
//
//    ISample** sample = (ISample**)SampleImage::createSamples(inputData, outputData);
//    SampleData* sampleData = new SampleData(sample, inputData->countImages);
//
//    delete inputData->image;
//    delete outputData->image;
//
//    return sampleData;
//}

void SampleImage::realese()
{
    delete imageInput;
    delete imageOutput;
}
