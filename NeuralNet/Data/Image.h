#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <SOIL.h>
#include <string>

class Image
{
public:
	float* dev_image = nullptr; //Указатель на изображение в памяти видеокарты
	int sizeX;
	int sizeY;
	int channel;

	//Конструкторы
	Image();
	Image(float* image, int sizeX, int sizeY, int channel, bool inHost);
	Image(unsigned char* image, int sizeX, int sizeY, int channel);
	Image(const char* path, int channel = -1);

	//Диструктор
	~Image();

	//Основобождение памяти
	void freeMemory();

	//Загрузка изображения
	void loadImage(const char* path, int channel = -1);
	void loadImage(float* image, int sizeX, int sizeY, int channel, bool inHost);
	void loadImage(unsigned char* image, int sizeX, int sizeY, int channel);

	//Получение изображения
	void getImage(float*& outImage);
	void getImage(unsigned char*& outImage);

	static void flToCh(float* fimage, unsigned char*& outChImage, int sizeX, int sizeY, int channel);
	static void chToFl(unsigned char* chImage, float*& outFImage, int sizeX, int sizeY, int channel);

	//Сохранение изображения
	void saveImage(const char* path);
};