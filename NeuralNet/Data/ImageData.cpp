#include "ImageData.h"

ImageData::ImageData()
{
	image = nullptr;
	countImages = 0;
}

ImageData::ImageData(char* path, int channel)
{
	loadImages(path, channel);
}

ImageData::~ImageData()
{
	realese();
}

void ImageData::realese()
{
	for (int x = 0; x < countImages; x++)
		delete image[x];
	delete image;
	image = nullptr;
	countImages = 0;
}

void ImageData::loadImages(char* path, int channel)
{
	if (image != nullptr)
		realese();
	char** names = PathManager::getFolderNames(path, &countImages);
	image = new Image*[countImages];
	for (int x = 0; x < countImages; x++)
		image[x] = new Image(names[x], channel);

	for (int x = 0; x < countImages; x++)
		delete names[x];
	delete names;
}

int ImageData::saveImages(char* path, const char* prename)
{
	if (countImages != 0)
	{
		if (path[strlen(path) - 1] != '*')
			return 2;

		int sizePrename = strlen(prename);
		int maxSizeName = log10(countImages) + 1;
		int sizeName = sizePrename + maxSizeName + 30;
		char* name = new char[sizeName];
		name[0] = '\\';
		name[1] = '\0';
		for (int x = 0; x < countImages; x++)
		{
			strcat(name, prename);
			PathManager::IntToString(x, name + sizePrename + 1);
			strcat(name, ".bmp");
			PathManager::addNameInstanse(path, 1, name);
			image[x]->saveImage(path);
		}

		PathManager::addNameInstanse(path, 1, "\\*");
		delete[sizeName] name;
		return 0;
	}
	else
	{
		return 1; //Ќе было загружено ни одного изображени€
	}
}
