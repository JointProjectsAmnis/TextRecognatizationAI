#include "Panel.h"

Panel::Panel(LPCWSTR lpWindowName, HBRUSH backgroundColor, BaseWindow* parent, HMENU hMenu, DWORD dwStyle) : BaseWindow(lpWindowName, dwStyle, backgroundColor, parent, hMenu, NULL)
{
    toCreateNewClass = true;
}

Panel::Panel(LPCWSTR lpWindowName, HBRUSH backgroundColor, int borderRadiusX, int borderRadiusY, BaseWindow* parent, HMENU hMenu, DWORD dwStyle) : BaseWindow(lpWindowName, dwStyle, backgroundColor, parent, hMenu, NULL)
{
    this->borderRadiusX = borderRadiusX;
    this->borderRadiusY = borderRadiusY;
}

LRESULT Panel::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        isClosed = true;
    }
    break;
    }

    if (uMsg == WM_PAINT)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (actionPaint.action != nullptr)
            actionPaint.action(this, ps, hdc, actionPaint.param);
        else
        {
            RECT rect;
            GetClientRect(hwnd, &rect);

            SelectObject(hdc, backgroundColor);
            SelectObject(hdc, invisePen);
            RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, borderRadiusX, borderRadiusY);

            EndPaint(hwnd, &ps);
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return BaseWindow::HandleMessage(hwnd, uMsg, wParam, lParam);
}

const TCHAR* Panel::ClassName()
{
    return L"Panel";
}