#include "SampleImageTest.h"

SampleImageTest::SampleImageTest()
{
}

SampleImageTest::SampleImageTest(Image* imageInput, Image* imageOutput)
{
    this->imageInput = imageInput;
    this->imageOutput = imageOutput;
}

SampleImageTest::~SampleImageTest()
{
    realese();
}

float* SampleImageTest::getInputData(int id)
{
    return imageInput->dev_image;
}

float* SampleImageTest::getOutputData(int id)
{
    return imageOutput->dev_image;
}

SampleImageTest** SampleImageTest::createSamples(ImageData* inputData, ImageData* outputData)
{
    int countSamples = inputData->countImages > outputData->countImages ? outputData->countImages : inputData->countImages;
    SampleImageTest** sample = new SampleImageTest * [countSamples];
    for (int x = 0; x < countSamples; x++)
        sample[x] = new SampleImageTest(inputData->image[x], outputData->image[x]);
    return sample;
}

//SampleData* SampleImageTest::createSampleData(char* pathInput, char* pathOutput)
//{
//    ImageData* inputData, *outputData;
//
//    inputData = new ImageData(pathInput);
//    outputData = new ImageData(pathOutput);
//
//    ISample** sample = (ISample**)SampleImageTest::createSamples(inputData, outputData);
//    SampleData* sampleData = new SampleData(sample, inputData->countImages);
//
//    delete inputData->image;
//    delete outputData->image;
//
//    return sampleData;
//}

void SampleImageTest::realese()
{
    delete imageInput;
    delete imageOutput;
}
