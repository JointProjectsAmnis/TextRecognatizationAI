#pragma once
#include "BaseWindow.h"

class Panel : public BaseWindow
{
public:

	Panel(
		LPCWSTR lpWindowName,
		HBRUSH backgroundColor,
		BaseWindow* parent = NULL,
		HMENU hMenu = AUTO_CODE,
		DWORD dwStyle = WS_OVERLAPPED);

protected:
	virtual LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual const TCHAR* ClassName() override;
};
