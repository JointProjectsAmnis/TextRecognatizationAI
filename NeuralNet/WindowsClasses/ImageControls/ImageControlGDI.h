#pragma once

#include "../BaseWindow.h"
#include "IImageControl.h"

class ImageControlGDI : public BaseWindow
{
public:

    int width = 0;
    int height = 0;

    HBITMAP bmp = NULL;

    ImageControlGDI(
        BaseWindow* parent = nullptr,
        HMENU hMenu = AUTO_CODE,
        DWORD dwExStyle = NULL);

    ~ImageControlGDI();

    void SetImage(int width, int height, const wchar_t* path);
    virtual const TCHAR* ClassName();
};

