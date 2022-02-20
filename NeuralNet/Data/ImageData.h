#pragma once
#include "../PathManager.h"
#include "Image.h"

class ImageData
{
public:

	int countImages = 0;
	Image** image = nullptr;

	/*
	*	Путь должен указывать на папку, а затем должно писать \\*.
	*   Пример: C:\\Images\\*
	*/
	ImageData();
	ImageData(char* path, int channel = -1);
	~ImageData();

	//Очищение памяти
	void realese();

	//Загрузка изобржений
	/*
	*	Путь должен указывать на папку, а затем должно писать \\*.
	*   Пример: C:\\Images\\*
	*/
	void loadImages(char* path, int channel = -1);

	/*
	* Сохранить изображения в путь path и с именем prename, после
	* которого идёт цифра - номер картинки.
	* 
	* Путь должен указывать на папку, а затем должно писать \\*.
	* Пример: C:\\Images\\*
	* 
	* Возвращается ошибка:
	* 0 - нет ошибки
	* 1 - не было загружено ни одного изображения
	* 2 - не верно указан формат пути (в конце нужно ставить //*)
	*/
	int saveImages(char* path, const char* prename);
};

