#include "Commanded.h"

Commanded::Commanded(
	LPCWSTR lpWindowName, 
	DWORD dwStyle, 
	HBRUSH backgroundColor, 
	BaseWindow* parent, 
	HMENU hMenu, 
	DWORD dwExStyle)

	: BaseWindow(
		lpWindowName,
		dwStyle,
		backgroundColor,
		parent,
		hMenu,
		dwExStyle)
{
}

//Должны быть такие аргументы (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
void Commanded::AddAction(void* param, void(*action)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param))
{
	this->action.param = param;
	this->action.action = action;
}

LRESULT Commanded::InvokeAction(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (action.action)
		action.action(hwnd, uMsg, wParam, lParam, action.param);
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
