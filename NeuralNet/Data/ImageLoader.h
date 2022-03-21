#pragma once
#include "PathManager.h"
#include <soil.h>

class ImageLoader
{
public:

	int countImages;
	int sizeImageX;
	int sizeImageY;
	int channels;
	unsigned char** imageByte;
	double** imageDouble;

	ImageLoader(char* path, int force_channels);
	~ImageLoader();
};

