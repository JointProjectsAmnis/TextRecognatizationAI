#pragma once
#include <windows.h>

#define NONCOLOR -1

class Brush
{
public:

	Brush();
	Brush(COLORREF color);
	Brush(HBRUSH brush);
	~Brush();

	COLORREF GetColor();
	HBRUSH GetBrush();

private:
	HBRUSH brush;
	COLORREF color = -1;
};

static Brush brushDefault = Brush(RGB(255, 255, 255));