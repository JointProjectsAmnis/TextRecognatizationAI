#pragma once
#include  "../Data/SampleData.h"

class INeuralNet
{
public:

	virtual void forwardPropogation(void* input) = 0;
	virtual void backPropogation(void* teather) = 0;

	virtual void learnOne(ISample* sample) = 0;
	virtual void learn(SampleData* sampleData, int countGenerations) = 0;
	/*
	* ���������� ������, ������� ������ 
	* ��������� ���� ����� forwardPropogation
	*/
	virtual double* getOutput() = 0;

	/*
	*outImage - �������� ������������
	*outImage ��������� �� ��� ���������� ������ ��� �����������.
	*���� �� outImage ����� ����� nullpt, �� ������ ��� ���� ��������� ����.
	*/
	virtual Image* getImage(Image*& outImage) = 0;

	virtual int getCountLayers() = 0;
	virtual int getCountNeurons(int id) = 0;
};