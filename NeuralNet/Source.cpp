#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <iostream>
#include "WindowsClasses/Panel.h"
#include "WindowsClasses/PanelContext.h"
#include "WindowsClasses/Buttons/Button.h"
#include "Source.h"
#include "PathManager.h"

HBRUSH brushBackground = CreateSolidBrush(RGB(26, 32, 48));
HBRUSH brushWhite = CreateSolidBrush(RGB(255, 255, 255));

HBRUSH brushIdentity = CreateSolidBrush(RGB(45, 49, 71));
HBRUSH brushHot = CreateSolidBrush(RGB(55, 59, 79));
HBRUSH brushSelected = CreateSolidBrush(RGB(45, 49, 71));

HBRUSH brushIdentityBlue = CreateSolidBrush(RGB(76, 194, 255));
HBRUSH brushHotBlue = CreateSolidBrush(RGB(71, 177, 234));
HBRUSH brushSelectedBlue = CreateSolidBrush(RGB(65, 161, 214));

HFONT font;

void main()
{
	int countX = 10;
	int countY = 10;

	int sizeButtonX = 50;
	int sizeButtonY = 30;

	font = CreateFont(
		15, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

	Panel windowMain = Panel(L"Sample window", brushBackground, NULL, NULL, WS_OVERLAPPEDWINDOW);
	windowMain.Create(CW_USEDEFAULT, CW_USEDEFAULT, 590, 350);

	PanelContext* context = new PanelContext(L"Context", brushIdentity, &windowMain);
	context->Create(8, 8, 250, 250);
	context->Show(1);

	PanelContext* textPanel = new PanelContext(L"Context1", brushIdentity, &windowMain);
	textPanel->Create(8 + 300 + 8, 8, 250, 250);
	textPanel->Show(1);

	Button* button = new Button(L"Choose image", brushIdentity, brushHot, brushSelected, &windowMain);
	button->Create(8, 275, 100, 20);
	button->SetFont(font, RGB(255, 255, 255));
	button->Show(1);


	//TCHAR buttonText[256] = L"\0";

	//Button*** button = new Button**[countX];
	//for (int x = 0; x < countX; x++)
	//{
	//	button[x] = new Button*[countY];
	//	for (int y = 0; y < countY; y++)
	//	{
	//		TCHAR ch = L'1' + x;
	//		buttonText[0] = ch;
	//		buttonText[1] = L'\0';
	//		button[x][y] = new Button(buttonText, brushIdentity, brushHot, brushSelected, &windowMain);
	//		button[x][y]->Create(x * sizeButtonX, y * sizeButtonY, sizeButtonX, sizeButtonY);
	//		button[x][y]->SetFont(font, RGB(255, 255, 255));

	//		button[x][y]->AddAction((void*)ch, [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param)
	//			{
	//				std::cout << (char)param << std::endl;
	//			});
	//		button[x][y]->Show(1);
	//	}
	//}

	windowMain.Show(1);


	MSG msg = {};
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}