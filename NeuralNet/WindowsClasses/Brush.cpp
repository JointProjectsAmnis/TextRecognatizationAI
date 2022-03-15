#include "Brush.h"

Brush::Brush()
{
	color = brushDefault.GetColor();
	brush = brushDefault.GetBrush();
}

Brush::Brush(Brush::Color color)
{
	this->colorRGB = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };
	this->color = RGB(color.r, color.g, color.b);
	this->brush = CreateSolidBrush(this->color);
}

Brush::Brush(HBRUSH brush)
{
	this->brush = brush;
}

Brush::~Brush()
{
	DeleteObject(brush);
}

COLORREF Brush::GetColor()
{
	if (color == NONCOLOR)
		throw;
	else
		return color;
}

Brush::Color Brush::GetColorRGB()
{
	if (color == NONCOLOR)
		throw;
	else
		return colorRGB;
}

HBRUSH Brush::GetBrush()
{
	return brush;
}
