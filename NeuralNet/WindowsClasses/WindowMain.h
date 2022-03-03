#pragma once
#include "BaseWindow.h"

class WindowMain : public BaseWindow
{
public:

	WindowMain(
        LPCWSTR lpWindowName,
		HBRUSH backgroundColor,
        HWND hWndParent = NULL,
        HMENU hMenu = AUTO_CODE,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW);

protected:
	virtual LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual const TCHAR* ClassName() override;
};

