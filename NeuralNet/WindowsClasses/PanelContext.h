#pragma once
#include "Panel.h"

class PanelContext : public Panel
{
public:
	PanelContext(LPCWSTR lpWindowName, HBRUSH backgroundColor, BaseWindow* parent, HMENU hMenu = AUTO_CODE, DWORD dwStyle = NULL) :
		Panel(lpWindowName, backgroundColor, parent, hMenu, dwStyle) {}

	virtual LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override
	{
		static HGDIOBJ invisePen = CreatePen(PS_NULL, 0, 0);
		if (uMsg == WM_PAINT)
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			SelectObject(hdc, backgroundColor);
			SelectObject(hdc, invisePen);
			RoundRect(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom, 8, 8);

			EndPaint(hwnd, &ps);
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		Panel::HandleMessage(hwnd, uMsg, wParam, lParam);
	}
};
