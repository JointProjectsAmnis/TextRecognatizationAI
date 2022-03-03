#include "WindowMain.h"

WindowMain::WindowMain(LPCWSTR lpWindowName, HBRUSH backgroundColor, HWND hWndParent, HMENU hMenu, DWORD dwStyle) : WindowMain::BaseWindow(lpWindowName, dwStyle, backgroundColor, hWndParent, hMenu, NULL)
{

}

LRESULT WindowMain::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

const TCHAR* WindowMain::ClassName()
{
    return L"WindowMain";
}