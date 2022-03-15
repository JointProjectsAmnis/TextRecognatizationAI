#include "ButtonChooserImage.h"

ButtonChooserImage::ButtonChooserImage(
	LPCWSTR buttonText, 
	HGDIOBJ brushIdentity, 
	HGDIOBJ brushHot, 
	HGDIOBJ brushSelected, 
	BaseWindow* parent, 
	IFileOpenDialog* openDialog,
	HMENU hMenu) 

	: Button(
	buttonText,
	brushIdentity,
	brushHot,
	brushSelected,
	parent,
	hMenu)
{
	this->openDialog = openDialog;

	this->texture = nullptr;
	this->graphics = nullptr;

	this->image = nullptr;
	this->width = 5;
	this->height = 0;

	void** param = new void*[]{openDialog, this, &texture, &graphics, &image, &width, &height};
	AddAction(param, ButtonChooserImage::ToChooseImage);
}

ButtonChooserImage::~ButtonChooserImage()
{
	delete texture;
	delete[width * height * channel] image;
}

void ButtonChooserImage::SetGraphics(Graphics* graphics)
{
	this->texture = texture;
	this->graphics = graphics;
}

void ButtonChooserImage::ToChooseImage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param)
{
	IFileOpenDialog* openDialog = (IFileOpenDialog*)(*(void**)param);
	Button* button = (Button*)(*((void**)param + 1));
	Texture** texture = (Texture**)(*((void**)param + 2));
	Graphics** graphics = (Graphics**)(*((void**)param + 3));
	unsigned char** image = (unsigned char**)(*((void**)param + 4));
	int* width = (int*)(*((void**)param + 5));
	int* height = (int*)(*((void**)param + 6));

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

		if (*image)
		{
			free(*image);
			*image = nullptr;
		}

		if (*graphics != nullptr)
		{
			int channels;
			*image = SOIL_load_image(path, width, height, &channels, 4);

			if (*texture == nullptr)
			{
				*texture = new Texture(*graphics, *width, *height, 4, *image);
			}
			else
			{
				texture[0]->SetTexture(*width, *height, channels, *image);
			}
		}
		else
		{
			throw; //Нужно сначала юзануть метод SetGraphics()!!!
		}

		free(path);

		text = PathManager::getLastName(text, 1);

		int maxSize = 13;
		if (lstrlenW(text) > maxSize)
		{
			textMaxSize = (LPWSTR)calloc(maxSize + 4, sizeof(WCHAR));
			memcpy(textMaxSize, text, sizeof(WCHAR) * maxSize);

			textMaxSize[maxSize] = L'.';
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
}

LRESULT ButtonChooserImage::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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

		if(image)
			free(image);

		int channels;
		image = SOIL_load_image(path, &width, &height, &channels, 4);

		if (texture == nullptr)
		{
			texture = new Texture(graphics, width, height, channels, image);
		}
		else
		{
			texture->SetTexture(width, height, channels, image);
		}

		free(path);

		text = PathManager::getLastName(text, 1);

		int maxSize = 13;
		if (lstrlenW(text) > maxSize)
		{
			textMaxSize = (LPWSTR)calloc(maxSize + 4, sizeof(WCHAR));
			memcpy(textMaxSize, text, sizeof(WCHAR) * maxSize);

			textMaxSize[maxSize] = L'.';
			textMaxSize[maxSize + 1] = L'.';
			textMaxSize[maxSize + 2] = L'.';
			textMaxSize[maxSize + 3] = L'\0';

			SetText(textMaxSize);
			free(textMaxSize);
		}
		else
		{
			SetText(text);
		}
	}
	return Button::HandleMessage(hwnd, uMsg, wParam, lParam);
}
