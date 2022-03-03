#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <iostream>
#include "WindowsClasses/WindowMain.h"
#include "WindowsClasses/Buttons/Button.h"
#include "Source.h"
#include "PathManager.h"

HBRUSH brushBackground = CreateSolidBrush(RGB(26, 32, 48));

HBRUSH brushIdentity = CreateSolidBrush(RGB(45, 49, 71));
HBRUSH brushHot = CreateSolidBrush(RGB(55, 59, 79));
HBRUSH brushSelected = CreateSolidBrush(RGB(45, 49, 71));

HFONT font;

void main()
{
	int countX = 10;
	int countY = 10;

	int sizeButtonX = 50;
	int sizeButtonY = 30;

	font = CreateFont(
		20, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

	WindowMain windowMain = WindowMain(L"Sample window", brushBackground);
	windowMain.Create(CW_USEDEFAULT, CW_USEDEFAULT, countX * sizeButtonX + 15, (countY + 1) * sizeButtonY + 10);

	TCHAR buttonText[256] = L"\0";

	Button*** button = new Button**[countX];
	for (int x = 0; x < countX; x++)
	{
		button[x] = new Button*[countY];
		for (int y = 0; y < countY; y++)
		{
			TCHAR ch = L'1' + x;
			buttonText[0] = ch;
			buttonText[1] = L'\0';
			button[x][y] = new Button(buttonText, brushIdentity, brushHot, brushSelected, &windowMain);
			button[x][y]->Create(x * sizeButtonX, y * sizeButtonY, sizeButtonX, sizeButtonY);
			button[x][y]->SetFont(font, RGB(255, 255, 255));

			button[x][y]->AddAction((void*)ch, [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param)
				{
					std::cout << (char)param << std::endl;
				});
			button[x][y]->Show(1);
		}
	}

	windowMain.Show(1);

	//Button button = Button(L"C", brushIdentity, brushHot, brushSelected, &windowMain);
	//button.Create(100, 100, 100, 50);
	//button.Show(1);

	//button.AddAction(&windowMain, [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param)
	//	{
	//		std::cout << "You press button 1!!!" << std::endl;
	//	});

	//button.SetFont(font, RGB(255, 255, 255));

	//Button button1 = Button(L"How are you?", brushIdentity, brushHot, brushSelected, &windowMain);
	//button1.Create(300, 100, 100, 50);
	//button1.Show(1);

	//button1.AddAction(nullptr, [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param)
	//	{
	//		std::cout << "You press button 2!!!" << std::endl;
	//	});

	//button1.SetFont(font, RGB(255, 255, 255));

	MSG msg = {};
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}