#include "ImageControlGDI.h"

ImageControlGDI::ImageControlGDI(
	BaseWindow* parent,
	HMENU hMenu, 
	DWORD dwExStyle)

	: BaseWindow(
		L" ",
		WS_VISIBLE | SS_BITMAP,
		WHITE_BRUSH,
		parent,
		hMenu,
		dwExStyle)
{
	this->width = width;
	this->height = height;
}

ImageControlGDI::~ImageControlGDI()
{
	DeleteObject(bmp);
}

void ImageControlGDI::SetImage(int width, int height, const wchar_t* path)
{
	this->width = width;
	this->height = height;
	bmp = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	SendMessage(hwnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
}

const TCHAR* ImageControlGDI::ClassName()
{
	return L"static";
}
