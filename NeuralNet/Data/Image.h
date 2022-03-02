#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <SOIL.h>
#include <string>

class Image
{
public:
	double* dev_image = nullptr; //��������� �� ����������� � ������ ����������
	int sizeX;
	int sizeY;
	int channel;

	//������������
	Image();
	Image(double* image, int sizeX, int sizeY, int channel, bool inHost);
	Image(unsigned char* image, int sizeX, int sizeY, int channel);
	Image(const char* path, int channel = -1);

	//����������
	~Image();

	//�������������� ������
	void freeMemory();

	//�������� �����������
	void loadImage(const char* path, int channel = -1);
	void loadImage(double* image, int sizeX, int sizeY, int channel, bool inHost);
	void loadImage(unsigned char* image, int sizeX, int sizeY, int channel);

	//��������� �����������
	void getImage(double*& outImage);
	void getImage(unsigned char*& outImage);

	static void flToCh(double* fimage, unsigned char*& outChImage, int sizeX, int sizeY, int channel);
	static void chToFl(unsigned char* chImage, double*& outFImage, int sizeX, int sizeY, int channel);

	//���������� �����������
	void saveImage(const char* path);
};