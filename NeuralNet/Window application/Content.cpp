#include "Content.h"

//Êטסעט
Brush* Content::brushBackground;
Brush* Content::brushWhite;

Brush* Content::brushIdentity;
Brush* Content::brushHot;
Brush* Content::brushSelected;

Brush* Content::brushIdentityBlue;
Brush* Content::brushHotBlue;
Brush* Content::brushSelectedBlue;

//״נטפעû
HFONT* Content::font;

void Content::Load()
{
	//Êטסעט
	Content::brushBackground = new Brush({ 26, 32, 48 });
	Content::brushWhite = new Brush({ 255, 255, 255 });

	Content::brushIdentity = new Brush({ 45, 49, 71 });
	Content::brushHot = new Brush({ 55, 59, 79 });
	Content::brushSelected = new Brush({ 45, 49, 71 });

	Content::brushIdentityBlue = new Brush({ 76, 194, 255 });
	Content::brushHotBlue = new Brush({ 71, 177, 234 });
	Content::brushSelectedBlue = new Brush({ 65, 161, 214 });

	//״נטפעû
	Content::font = new HFONT(CreateFont(
		15, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI"));
}