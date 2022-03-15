#pragma once

#include <shobjidl.h>


#include <SOIL.h>

//WindowClasses
#include "../WindowsClasses/Buttons/Button.h"

//DirectX
#include "../WindowsClasses/DirectX3D/Texture.h"
#include "../WindowsClasses/DirectX3D/Graphics.h"

#include "../PathManager.h"

class ButtonChooserImage : public Button
{
public:
	IFileOpenDialog* openDialog;

	Texture* texture;
	Graphics* graphics;

	int width;
	int height;
	const int channel = 4;
	unsigned char* image;

	ButtonChooserImage(
		LPCWSTR buttonText,
		HGDIOBJ brushIdentity,
		HGDIOBJ brushHot,
		HGDIOBJ brushSelected,
		BaseWindow* parent,
		IFileOpenDialog* openDialog,
		HMENU hMenu = AUTO_CODE);

	~ButtonChooserImage();

	void SetGraphics(Graphics* graphics);
	void static ToChooseImage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param);

	LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

