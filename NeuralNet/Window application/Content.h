#pragma once

#include "../WindowsClasses/Brush.h"

class Content
{
public:
	//�����
	static Brush* brushBackground;
	static Brush* brushWhite;

	static Brush* brushIdentity;
	static Brush* brushHot;
	static Brush* brushSelected;

	static Brush* brushIdentityBlue;
	static Brush* brushHotBlue;
	static Brush* brushSelectedBlue;


	//������
	static HFONT* font;

	static void Load();
};

