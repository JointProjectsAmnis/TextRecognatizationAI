#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <shobjidl.h>
#include <iostream>
#include "WindowsClasses/Panel.h"
#include "WindowsClasses/PanelContext.h"
#include "WindowsClasses/Buttons/Button.h"
#include "WindowsClasses/Lable.h"
#include "WindowsClasses/Brush.h"
#include "Source.h"
#include "PathManager.h"

Brush brushBackground = Brush(RGB(26, 32, 48));
Brush brushWhite = Brush(RGB(255, 255, 255));

Brush brushIdentity = Brush(RGB(45, 49, 71));
Brush brushHot = Brush(RGB(55, 59, 79));
Brush brushSelected = Brush(RGB(45, 49, 71));

Brush brushIdentityBlue = Brush(RGB(76, 194, 255));
Brush brushHotBlue = Brush(RGB(71, 177, 234));
Brush brushSelectedBlue = Brush(RGB(65, 161, 214));

HFONT font;

void main()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	int countX = 10;
	int countY = 10;

	int sizeButtonX = 50;
	int sizeButtonY = 30;

	font = CreateFont(
		15, 0, 0, 0, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

	Panel* windowMain = new Panel(L"Sample window", brushBackground.GetBrush(), NULL, NULL, WS_OVERLAPPEDWINDOW);
	windowMain->Create(CW_USEDEFAULT, CW_USEDEFAULT, 590, 350);

	PanelContext* context = new PanelContext(L"Context", brushIdentity.GetBrush(), windowMain);
	context->Create(8, 8, 250, 250);
	context->Show(1);

	PanelContext* textPanel = new PanelContext(L"Context1", brushIdentity.GetBrush(), windowMain);
	textPanel->Create(8 + 300 + 8, 8, 250, 250);
	textPanel->Show(1);

	IFileOpenDialog* openDialog;
	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&openDialog);

	Lable* lable = new Lable(L"Hello, world!", RGB(255, 255, 255), font, windowMain);
	lable->Create(115, 275, 100, 20);
	lable->Show(1);

	Button* button = new Button(L"Choose image", brushIdentity.GetBrush(), brushHot.GetBrush(), brushSelected.GetBrush(), windowMain);
	button->Create(8, 275, 100, 20);
	button->SetFont(font, RGB(255, 255, 255));
	void* actionData[] = {openDialog, lable};
	button->AddAction(actionData, [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param)
		{
			IFileOpenDialog* openDialog = (IFileOpenDialog*)(*(void**)param);
			Lable* lable = (Lable*)(*((void**)param + 1));
			openDialog->Show(NULL);
			IShellItem* item;
			openDialog->GetResult(&item);
			//WCHAR text[500] = L"123";
			LPWSTR text;
			item->GetDisplayName(SIGDN_FILESYSPATH, &text);
			lable->SetText(PathManager::getLastName(text, 1));

		});
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

	windowMain->Show(1);


	MSG msg = {};
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}