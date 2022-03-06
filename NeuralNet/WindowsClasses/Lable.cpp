#include "Lable.h"

Lable::Lable(
	LPCWSTR text,
	COLORREF colorText,
	HFONT font,
	BaseWindow* parent, 
	HMENU hMenu, 
	DWORD dwExStyle)

	: BaseWindow(
		text,
		WS_VISIBLE,
		WHITE_BRUSH,
		parent,
		hMenu,
		dwExStyle)
{
	SetFont(font, colorText);
}

const TCHAR* Lable::ClassName()
{
	return L"static";
}

LRESULT Lable::HandleNotifyStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC)wParam;
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, colorFont);
	SelectObject(hdc, font);
	return parent ? (LRESULT)parent->backgroundColor : (LRESULT)backgroundColor;
}
