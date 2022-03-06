#pragma once
#include "../Decl.h"
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
	DECL ImageData();
	DECL ImageData(char* path, int channel = -1);
	DECL ~ImageData();

	//�������� ������
	DECL void realese();

	//�������� ����������
	/*
	*	���� ������ ��������� �� �����, � ����� ������ ������ \\*.
	*   ������: C:\\Images\\*
	*/
	DECL void loadImages(char* path, int channel = -1);

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
	DECL int saveImages(char* path, const char* prename);
};

