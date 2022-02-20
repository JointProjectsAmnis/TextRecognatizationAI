#pragma once
#include "../PathManager.h"
#include "Image.h"

class ImageData
{
public:

	int countImages = 0;
	Image** image = nullptr;

	/*
	*	���� ������ ��������� �� �����, � ����� ������ ������ \\*.
	*   ������: C:\\Images\\*
	*/
	ImageData();
	ImageData(char* path, int channel = -1);
	~ImageData();

	//�������� ������
	void realese();

	//�������� ����������
	/*
	*	���� ������ ��������� �� �����, � ����� ������ ������ \\*.
	*   ������: C:\\Images\\*
	*/
	void loadImages(char* path, int channel = -1);

	/*
	* ��������� ����������� � ���� path � � ������ prename, �����
	* �������� ��� ����� - ����� ��������.
	* 
	* ���� ������ ��������� �� �����, � ����� ������ ������ \\*.
	* ������: C:\\Images\\*
	* 
	* ������������ ������:
	* 0 - ��� ������
	* 1 - �� ���� ��������� �� ������ �����������
	* 2 - �� ����� ������ ������ ���� (� ����� ����� ������� //*)
	*/
	int saveImages(char* path, const char* prename);
};

