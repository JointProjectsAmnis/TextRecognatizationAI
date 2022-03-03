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
	double* image;
	dim3 dimX, dimY;

	NeuralNetDebug(int sizeX, int sizeY, int channel);

	virtual void forwardPropogation(void* input) override;
	virtual void backPropogation(void* teather) override;

	virtual void learnOne(ISample* sample) override;
	virtual void learn(SampleData* sampleData, int countGenerations) override;

	virtual double* getOutput() override;
	virtual Image* getImage(Image*& outImage) override;


	virtual int getCountLayers() override;
	virtual int getCountNeurons(int id) override;

};

