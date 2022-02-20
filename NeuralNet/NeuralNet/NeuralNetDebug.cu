
#pragma once

#include "NeuralNetDebug.cuh"
#define COUNT_THREADS 16
#define COUNT_BLOCKS(N, TH) (unsigned int)((N + TH - 1) / TH)

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <thread>

__global__ void lerpImage(float* image1, float* image2, int sizeX, int sizeY, int channel)
{
    int idX = (threadIdx.x + blockIdx.x * blockDim.x) * channel;
    int idY = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = idX + idY * gridDim.x * blockDim.x * channel;

    if (offset < sizeX * sizeY * channel)
        for (int x = 0; x < channel; x++)
            image1[offset + x] = image1[offset + x] + (image2[offset + x] - image1[offset + x]) * 0.5f;
}

NeuralNetDebug::NeuralNetDebug(int sizeX, int sizeY, int channel)
{
    image = nullptr;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->channel = channel;

    dimX = { COUNT_BLOCKS(sizeX, COUNT_THREADS), COUNT_BLOCKS(sizeY, COUNT_THREADS) };
    dimY = { COUNT_THREADS, COUNT_THREADS };
}

void NeuralNetDebug::forwardPropogation(float* input)
{
    this->image = input;
}

void NeuralNetDebug::backPropogation(float* teather)
{
    lerpImage << < dimX, dimY >> > (image, teather, sizeX, sizeY, channel);
}

void NeuralNetDebug::learnOne(ISample* sample)
{
}

void NeuralNetDebug::learn(SampleData* sampleData, int countGenerations)
{
}

float* NeuralNetDebug::getOutput()
{
    return image;
}

Image* NeuralNetDebug::getImage(Image*& outImage)
{
    if (outImage == nullptr)
        outImage = new Image(image, sizeX, sizeY, channel, false);
    return outImage;
}

int NeuralNetDebug::getCountLayers()
{
    return 1;
}

int NeuralNetDebug::getCountNeurons(int id)
{
    return sizeX * sizeY * channel;
}