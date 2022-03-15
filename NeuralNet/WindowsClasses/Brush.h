#pragma once
#include <windows.h>

#define NONCOLOR -1

class Brush
{
public:

	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};

	Brush();
	Brush(Color color);
	Brush(HBRUSH brush);
	~Brush();

	COLORREF GetColor();
	Color GetColorRGB();
	HBRUSH GetBrush();

	HBRUSH brush;
	COLORREF color = -1;
	Color colorRGB = {};
};

static Brush brushDefault = Brush({ 1, 1, 1, 1 });