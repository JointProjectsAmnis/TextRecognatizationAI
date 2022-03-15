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

	//������
	Panel* windowMain;
	Panel* context;
	Panel* textPanel;

	//������
	Lable* lable;

	//������
	ButtonChooserImage* button;

	//���������� ����
	IFileOpenDialog* openDialog;

	//�����
	Brush* brushBackground;
	Brush* brushIdentity;
	Brush* brushHot;
	Brush* brushSelected;

	//�������
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

