#pragma once
#include "INeuralNet.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <thread>
class NeuralNetDebug : public INeuralNet
{
public:

	int sizeX, sizeY, channel;
	float* image;
	dim3 dimX, dimY;

	NeuralNetDebug(int sizeX, int sizeY, int channel);

	virtual void forwardPropogation(float* input) override;
	virtual void backPropogation(float* teather) override;

	virtual void learnOne(ISample* sample) override;
	virtual void learn(SampleData* sampleData, int countGenerations) override;

	virtual float* getOutput() override;
	virtual Image* getImage(Image*& outImage) override;


	virtual int getCountLayers() override;
	virtual int getCountNeurons(int id) override;

};

