#pragma once
#include <windows.h>

class IHandleNotify
{
public:
	virtual LRESULT HandleNotify(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LPNMHDR someItem) = 0;
};