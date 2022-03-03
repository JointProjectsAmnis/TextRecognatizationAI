#pragma once
#include "..//BaseWindow.h"
#include "..//Action.h"

class Button : public BaseWindow
{
public:

	HGDIOBJ brushIdentity;
	HGDIOBJ brushHot;
	HGDIOBJ brushSelected;

	BaseWindow* parent;
	Action action = {0};
	Button(
		LPCWSTR buttonText,
		HGDIOBJ brushIdentity,
		HGDIOBJ brushHot,
		HGDIOBJ brushSelected,
		BaseWindow* parent,
		HMENU hMenu = AUTO_CODE);

	void AddAction(void* param, void(*action)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param));
	LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual LRESULT HandleNotify(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LPNMHDR someItem) override;
	virtual const TCHAR* ClassName();

};

