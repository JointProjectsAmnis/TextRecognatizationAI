#include "Panel.h"

Panel::Panel(LPCWSTR lpWindowName, HBRUSH backgroundColor, BaseWindow* parent, HMENU hMenu, DWORD dwStyle) : BaseWindow(lpWindowName, dwStyle, backgroundColor, parent, hMenu, NULL)
{
    toCreateNewClass = true;
}

LRESULT Panel::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
    }
    break;
    }

    return BaseWindow::HandleMessage(hwnd, uMsg, wParam, lParam);
}

const TCHAR* Panel::ClassName()
{
    return L"Panel";
}