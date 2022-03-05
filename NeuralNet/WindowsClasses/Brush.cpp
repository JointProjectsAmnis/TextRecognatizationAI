#include "Brush.h"

Brush::Brush()
{
	color = brushDefault.GetColor();
	brush = brushDefault.GetBrush();
}

Brush::Brush(COLORREF color)
{
	this->color = color;
	this->brush = CreateSolidBrush(color);
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

HBRUSH Brush::GetBrush()
{
	return brush;
}
