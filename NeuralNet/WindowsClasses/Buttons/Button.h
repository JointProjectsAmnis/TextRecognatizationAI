#pragma once
#include "..//Commanded.h"
#include "..//IHandleNotify.h"

class Button : public Commanded, public IHandleNotify
{
public:

	HGDIOBJ brushIdentity;
	HGDIOBJ brushHot;
	HGDIOBJ brushSelected;

	Button(
		LPCWSTR buttonText,
		HGDIOBJ brushIdentity,
		HGDIOBJ brushHot,
		HGDIOBJ brushSelected,
		BaseWindow* parent,
		HMENU hMenu = AUTO_CODE);

	virtual LRESULT HandleNotify(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LPNMHDR someItem) override;
	virtual const TCHAR* ClassName();
};

