#include "ImageLoader.h"

ImageLoader::ImageLoader(char* path, int force_channels)
{
	this->channels = force_channels;
	char** names = PathManager::getFolderNames(path, &countImages);
	
	imageByte = new unsigned char*[countImages];
	imageDouble = new double*[countImages];

	for (int x = 0; x < countImages; x++)
	{
		int channels;
		imageByte[x] = SOIL_load_image(names[x], &sizeImageX, &sizeImageY, &channels, force_channels);
		imageDouble[x] = new double[sizeImageX * sizeImageY * force_channels];
		for (int i = 0; i < sizeImageX * sizeImageY * force_channels; i++)
			imageDouble[x][i] = imageByte[x][i] / 255.0f;
	}

	delete[countImages] names;
}

ImageLoader::~ImageLoader()
{
	for (int x = 0; x < countImages; x++)
	{
		delete[sizeImageX * sizeImageY * channels] imageByte[x];
		delete[sizeImageX * sizeImageY * channels] imageDouble[x];
	}

	delete[countImages] imageByte;
	delete[countImages] imageDouble;
}
