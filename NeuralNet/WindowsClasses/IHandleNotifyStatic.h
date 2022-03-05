#pragma once
#include <windows.h>

class IHandleNotifyStatic
{
public:
	virtual LRESULT HandleNotifyStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};