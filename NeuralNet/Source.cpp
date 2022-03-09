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
#include "WindowsClasses/ImageControls/ImageControlGDI.h"

#include "WindowsClasses/DirectX3D/Graphics.h"
#include "WindowsClasses/DirectX3D/Shaders/Shader.h"
#include "WindowsClasses/DirectX3D/Shaders/RegisterShaders.h"

#include "WindowsClasses/DirectX3D/VertexBuffer.h"
#include "WindowsClasses/DirectX3D/ConstantBuffer.h"
#include "WindowsClasses/DirectX3D/Texture.h"
#include "WindowsClasses/DirectX3D/Sampler.h"

#include "Source.h"
#include "PathManager.h"

#include <string>

#include <SOIL.h>

#define MAX_SIZE 2000

struct Vertex
{
	FLOAT x, y, z;
	FLOAT color[4];
};

Vertex verticesColor[] =
{
	{-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
	{ 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
	{ 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f},

	{ 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
	{-1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
	{-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
};

Vertex vertices[] =
{
	{-1.0f, 1.0f, 0.0f},
	{ 1.0f, 1.0f, 0.0f},
	{ 1.0f,-1.0f, 0.0f},

	{ 1.0f,-1.0f, 0.0f},
	{-1.0f,-1.0f, 0.0f},
	{-1.0f, 1.0f, 0.0f},
};

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
	setlocale(LC_ALL, "ru");
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	int countX = 10;
	int countY = 10;

	int sizeButtonX = 50;
	int sizeButtonY = 30;

	wchar_t path[MAX_SIZE];
	wchar_t pathFile[MAX_SIZE];
	PathManager::getFolderWithExe(path);

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

	COMDLG_FILTERSPEC rgSpec[] =
	{
		{L"jpg, png", L"*.jpg;*.jpeg;*.png"}
	};
	openDialog->SetFileTypes(1, rgSpec);

	//Lable* lable = new Lable(L"Hello, world!", RGB(255, 255, 255), font, windowMain);
	//lable->Create(115, 275, 100, 20);
	//lable->Show(1);

		//Создания класса для графики
	Graphics graphics = Graphics(context);
	RegisterShaders registerShaders = RegisterShaders(&graphics);
	VertexBuffer vertexBuffer = VertexBuffer(&graphics, vertices, sizeof(Vertex) * 6, sizeof(Vertex));

	int width, height, channels;
	unsigned char* image = SOIL_load_image("E:\\Кодинг\\Изображения для обучения сети\\2.png", &width, &height, &channels, 0);

	Texture texture = Texture(&graphics, width, height, channels, image);
	Sampler sampler = Sampler(&graphics);

	Button* button = new Button(L"Выберите файл", brushIdentity.GetBrush(), brushHot.GetBrush(), brushSelected.GetBrush(), windowMain);
	button->Create(8, 275, 100, 20);
	button->SetFont(font, RGB(255, 255, 255));
	void* actionData[] = {openDialog, button, &texture};
	button->AddAction(actionData, [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param)
		{
			IFileOpenDialog* openDialog = (IFileOpenDialog*)(*(void**)param);
			Button* button = (Button*)(*((void**)param + 1));
			Texture* texture = (Texture*)(*((void**)param + 2));
			openDialog->Show(NULL);

			IShellItem* item = nullptr;
			openDialog->GetResult(&item);

			if (item)
			{

				LPWSTR text, textMaxSize;
				item->GetDisplayName(SIGDN_FILESYSPATH, &text);

				int lengthText = lstrlenW(text);
				char* path = new char[lengthText + 1];
				path[lengthText] = '\0';

				wcstombs(path, text, lengthText * sizeof(wchar_t));

				int width, height, channels;
				unsigned char* image = SOIL_load_image(path, &width, &height, &channels, 4);
				texture->SetTexture(width, height, channels, image);

				free(path);
				free(image);

				text = PathManager::getLastName(text, 1);

				int maxSize = 13;
				if (lstrlenW(text) > maxSize)
				{
					textMaxSize = (LPWSTR)calloc(maxSize + 4, sizeof(WCHAR));
					memcpy(textMaxSize, text, sizeof(WCHAR) * maxSize);

					textMaxSize[maxSize] =     L'.';
					textMaxSize[maxSize + 1] = L'.';
					textMaxSize[maxSize + 2] = L'.';
					textMaxSize[maxSize + 3] = L'\0';

					button->SetText(textMaxSize);
					free(textMaxSize);
				}
				else
				{
					button->SetText(text);
				}
			}
		});
	button->Show(1);

	windowMain->Show(1);


	RECT rect;
	GetClientRect(context->hwnd, &rect);
	float sizeCof[4] = { texture.width, texture.height, rect.right, rect.bottom };
	ConstantBuffer constantBuffer = ConstantBuffer(&graphics, &sizeCof, sizeof(float) * 4);


	FLOAT backgroundColor[4] = { 45 / 255.0f, 49 / 255.0f, 71 / 255.0f, 1 };
	MSG msg = {};
	while (!windowMain->isClosed)
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		graphics.Clear(backgroundColor);
		

		//k += 0.0001f;
		//constantBuffer.ToSetData(&k, sizeof(float));

		//RECT rect;
		//GetClientRect(context->hwnd, &rect);
		//float sizeCof[4] = { texture.width, texture.height, rect.right, rect.bottom };
		//constantBuffer.ToSetData(&sizeCof, sizeof(float) * 4);

		constantBuffer.SetBuffer(0);
		texture.Bind(0);
		sampler.Bind(0);
		graphics.Draw(registerShaders.shaderImage, &vertexBuffer);
		graphics.Display();
	}
}