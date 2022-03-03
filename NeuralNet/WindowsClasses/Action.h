#pragma once
#include <windows.h>


struct Action
{
	void* param;
	void(*action)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param);
};