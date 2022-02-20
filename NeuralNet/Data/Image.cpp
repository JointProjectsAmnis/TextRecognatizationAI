#include "Image.h"

Image::Image()
{
	sizeX = 0;
	sizeY = 0;
	channel = 0;
	dev_image = nullptr;
}

Image::Image(float* image, int sizeX, int sizeY, int channel, bool inHost)
{
	loadImage(image, sizeX, sizeY, channel, inHost);
}

Image::Image(unsigned char* image, int sizeX, int sizeY, int channel)
{
	loadImage(image, sizeX, sizeY, channel);
}

Image::Image(const char* path, int channel)
{
	loadImage(path, channel);
}

Image::~Image()
{
	freeMemory();
}

void Image::freeMemory()
{
	cudaError_t error = cudaFree(dev_image);
	dev_image = nullptr;
	if (error)
		throw;
}

/*
* »тоговое изображение сохран€етс€ в пам€ть видеокарты с
* типом float, а также нормализуетс€ в границах от 0 до 1.
*/
void Image::loadImage(const char* path, int channel)
{
	if (dev_image != nullptr)
		freeMemory();

	unsigned char* chImage;
	if(channel == -1)
		chImage = SOIL_load_image(path, &sizeX, &sizeY, &this->channel, SOIL_LOAD_AUTO);
	else
		chImage = SOIL_load_image(path, &sizeX, &sizeY, &this->channel, channel);

	float* image = new float[sizeX * sizeY * this->channel];
	for (int x = 0; x < sizeX * sizeY * this->channel; x++)
		image[x] = chImage[x] / 255.0f;

	delete[sizeX * sizeY * this->channel] chImage;

	cudaError_t error = cudaMalloc(&dev_image, sizeof(float) * sizeX * sizeY * this->channel);
	if (error != 0) throw;

	error = cudaMemcpy(dev_image, image, sizeof(float) * sizeX * sizeY * this->channel, cudaMemcpyHostToDevice);
	if (error != 0) throw;

	delete[sizeX * sizeY * this->channel] image;
}

void Image::loadImage(float* image, int sizeX, int sizeY, int channel, bool inHost)
{
	if (dev_image != nullptr)
		freeMemory();
	if (inHost)
	{
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		this->channel = channel;
		cudaError_t error = cudaMalloc(&dev_image, sizeof(float) * sizeX * sizeY * channel);
		if (error != 0) throw;

		error = cudaMemcpy(dev_image, image, sizeof(float) * sizeX * sizeY * channel, cudaMemcpyHostToDevice);
		if (error != 0) throw;
	}
	else
	{
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		this->channel = channel;
		dev_image = image;
	}
}

void Image::loadImage(unsigned char* image, int sizeX, int sizeY, int channel)
{
	if (dev_image != nullptr)
		freeMemory();
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->channel = channel;

	float* fimage = new float[sizeX * sizeY * channel];
	for (int x = 0; x < sizeX * sizeY * channel; x++)
		fimage[x] = image[x] / 255.0f;

	cudaError_t error = cudaMalloc(&dev_image, sizeof(float) * sizeX * sizeY * channel);
	if (error != 0) throw;

	error = cudaMemcpy(dev_image, fimage, sizeof(float) * sizeX * sizeY * channel, cudaMemcpyHostToDevice);
	if (error != 0) throw;

	delete[sizeX * sizeY * channel] fimage;
}

void Image::getImage(float*& outImage)
{
	if (outImage == nullptr)
		outImage = new float[sizeX * sizeY * channel];
	cudaError_t error = cudaMemcpy(outImage, dev_image, sizeof(float) * sizeX * sizeY * channel, cudaMemcpyDeviceToHost);
	if (error != 0) throw;
}

void Image::getImage(unsigned char*& outImage)
{
	float* fImage = nullptr;
	getImage(fImage);
	flToCh(fImage, outImage, sizeX, sizeY, channel);
	delete[sizeX * sizeY * channel] fImage;
}

void Image::flToCh(float* fimage, unsigned char*& outChImage, int sizeX, int sizeY, int channel)
{
	if (outChImage == nullptr)
		outChImage = new unsigned char[sizeX * sizeY * channel];
	for (int x = 0; x < sizeX * sizeY * channel; x++)
		outChImage[x] = fimage[x] * 255;
}

void Image::chToFl(unsigned char* chImage, float*& outFImage, int sizeX, int sizeY, int channel)
{
	if (outFImage == nullptr)
		outFImage = new float[sizeX * sizeY * channel];
	for (int x = 0; x < sizeX * sizeY * channel; x++)
		outFImage[x] = chImage[x] / 255.0f;
}

void Image::saveImage(const char* path)
{
	unsigned char* image = nullptr;
	getImage(image);
	SOIL_save_image(path, SOIL_SAVE_TYPE_BMP, sizeX, sizeY, channel, image);
	delete[sizeX * sizeY * channel] image;
}
