#include "BaseWindow.h"

HMENU BaseWindow::hMenuCounter = 0;

LRESULT CALLBACK BaseWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BaseWindow* window;
    if (uMsg == WM_NCCREATE) //Может быть ошибка
    {
        CREATESTRUCT* createStruct = (CREATESTRUCT*)lParam;
        window = (BaseWindow*)createStruct->lpCreateParams; //Может быть ошибка

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
    }
    else
    {
        window = (BaseWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (window == nullptr)
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    return window->HandleMessage(hwnd, uMsg, wParam, lParam);
}

BaseWindow::BaseWindow(
    LPCWSTR lpWindowName,
    DWORD dwStyle,
    HBRUSH backgroundColor,
    HWND hWndParent,
    HMENU hMenu,
    DWORD dwExStyle)
{
    this->lpWindowName = lpWindowName;
    this->dwStyle = dwStyle;
    this->backgroundColor = backgroundColor;
    this->hWndParent = hWndParent;
    this->dwExStyle = dwExStyle;
    this->hMenu = hMenu == (HMENU)(-1) ? hMenuCounter : hMenu;
    hMenuCounter = hMenu == (HMENU)(-1) ? (HMENU)((int)hMenuCounter + 1) : hMenuCounter;

    this->hwnd = NULL;
}

void BaseWindow::Create(
    int X,
    int Y,
    int nWidth,
    int nHeight)
{
    if (hwnd != NULL)
        return;

    if (toCreateNewClass)
    {
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.lpszClassName = ClassName();
        wc.hInstance = GetModuleHandle(NULL);
        wc.hbrBackground = backgroundColor;

        RegisterClass(&wc);
    }

    hwnd = CreateWindowEx(
        dwExStyle, 
        ClassName(), 
        lpWindowName, 
        dwStyle, 
        X, Y, 
        nWidth, 
        nHeight, 
        hWndParent, 
        hMenu, 
        GetModuleHandle(NULL), 
        (LPVOID)this);
}

void BaseWindow::Show(int nCmdShow)
{
    ShowWindow(hwnd, nCmdShow);
}

LRESULT BaseWindow::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_COMMAND)
        for (int x = 0; x < daughtWindows.size(); x++)
            if (wParam == (WPARAM)daughtWindows[x]->hMenu)
                return daughtWindows[x]->HandleMessage(hwnd, uMsg, wParam, lParam);

    if (uMsg == WM_NOTIFY)
    {
        LPNMHDR item = (LPNMHDR)lParam;
        for (int x = 0; x < daughtWindows.size(); x++)
            if (item->code == NM_CUSTOMDRAW)
                if (item->idFrom == (UINT_PTR)daughtWindows[x]->hMenu)
                    return daughtWindows[x]->HandleNotify(hwnd, uMsg, wParam, lParam, item);
    }

    if (uMsg == WM_CTLCOLORBTN)
    {
        return (LRESULT)backgroundColor;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void BaseWindow::AddWindow(BaseWindow* window)
{
    daughtWindows.push_back(window);
}

void BaseWindow::SetFont(HFONT font, COLORREF color)
{
    this->font = font;
    this->colorFont = color;
}

LRESULT BaseWindow::HandleNotify(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LPNMHDR someItem)
{
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
