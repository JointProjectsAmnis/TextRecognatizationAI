#pragma once

#include "../Decl.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <SOIL.h>
#include <string>

class Image
{
public:
	double* dev_image = nullptr; //Указатель на изображение в памяти видеокарты
	int sizeX;
	int sizeY;
	int channel;

	//Конструкторы
	DECL Image();
	DECL Image(double* image, int sizeX, int sizeY, int channel, bool inHost);
	DECL Image(unsigned char* image, int sizeX, int sizeY, int channel);
	DECL Image(const char* path, int channel = -1);

	//Диструктор
	DECL ~Image();

	//Основобождение памяти
	DECL void freeMemory();

	//Загрузка изображения
	DECL void loadImage(const char* path, int channel = -1);
	DECL void loadImage(double* image, int sizeX, int sizeY, int channel, bool inHost);
	DECL void loadImage(unsigned char* image, int sizeX, int sizeY, int channel);

	//Получение изображения
	DECL void getImage(double*& outImage);
	DECL void getImage(unsigned char*& outImage);

	DECL static void flToCh(double* fimage, unsigned char*& outChImage, int sizeX, int sizeY, int channel);
	DECL static void chToFl(unsigned char* chImage, double*& outFImage, int sizeX, int sizeY, int channel);

	//Сохранение изображения
	DECL void saveImage(const char* path);
};