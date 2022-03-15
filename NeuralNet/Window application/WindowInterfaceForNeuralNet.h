#pragma once

#include <shobjidl.h>

//WindowsClasses
#include "../WindowsClasses/Panel.h"
#include "../WindowsClasses/Lable.h"
#include "../WindowsClasses/Buttons/Button.h"
#include "../WindowsClasses/Brush.h"

//Windows application
#include "ButtonChooserImage.h"

class GraphicsContextImage;

class WindowInterfaceForNeuralNet
{
public:

	//Панели
	Panel* windowMain;
	Panel* context;
	Panel* textPanel;

	//Лейблы
	Lable* lable;

	//Кнопки
	ButtonChooserImage* button;

	//Диалоговые окна
	IFileOpenDialog* openDialog;

	//Кисти
	Brush* brushBackground;
	Brush* brushIdentity;
	Brush* brushHot;
	Brush* brushSelected;

	//Графика
	GraphicsContextImage* graphicsContext;

	WindowInterfaceForNeuralNet(
		Brush* brushBackground, 
		Brush* brushIdentity, 
		Brush* brushHot, 
		Brush* brushSelected, 
		HFONT font);

	~WindowInterfaceForNeuralNet();

	void SetGraphics(GraphicsContextImage* graphicsContext);
	void ShowWindowInterface(int nCmdShow);

	
};

