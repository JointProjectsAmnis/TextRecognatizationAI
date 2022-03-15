#include "BaseWindow.h"
#include "Commanded.h"

#include "IHandleNotify.h"
#include "IHandleNotifyStatic.h"

HMENU BaseWindow::hMenuCounter = 0;
HPEN BaseWindow::invisePen = CreatePen(PS_NULL, 0, NULL);

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
    BaseWindow* parent,
    HMENU hMenu,
    DWORD dwExStyle)
{
    this->lpWindowName = lpWindowName;
    this->backgroundColor = backgroundColor;
    this->parent = parent;
    this->dwExStyle = dwExStyle;
    this->hMenu = hMenu == (HMENU)(-1) ? hMenuCounter : hMenu;
    hMenuCounter = hMenu == (HMENU)(-1) ? (HMENU)((int)hMenuCounter + 1) : hMenuCounter;
    this->dwStyle = parent ? dwStyle | WS_CHILD : dwStyle;

    this->hwnd = NULL;

    if (parent)
        parent->AddWindow(this);
}

void BaseWindow::Create(
    int X,
    int Y,
    int nWidth,
    int nHeight)
{
    if (hwnd != NULL)
        return;

    this->posX = X;
    this->posY = Y;
    this->sizeX = nWidth;
    this->sizeY = nHeight;

    if (toCreateNewClass)
    {
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.lpszClassName = ClassName();
        wc.hInstance = GetModuleHandle(NULL);
        wc.hbrBackground = backgroundColor;

        RegisterClass(&wc);
    }

    //if (false)
    //{
    //    hwnd = CreateWindowEx(
    //        dwExStyle,
    //        ClassName(),
    //        lpWindowName,
    //        dwStyle,
    //        X, Y,
    //        nWidth,
    //        nHeight,
    //        parent ? parent->hwnd : NULL,
    //        hMenu,
    //        GetModuleHandle(NULL),
    //        (LPVOID)this);
    //}

    hwnd = CreateWindowEx(
        dwExStyle, 
        ClassName(), 
        lpWindowName, 
        dwStyle, 
        X, Y, 
        nWidth, 
        nHeight, 
        parent ? parent->hwnd : NULL,
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
    switch (uMsg)
    {

    case WM_COMMAND:
    {
        for (int x = 0; x < childWindow.size(); x++)
            if (wParam == (WPARAM)childWindow[x]->hMenu)
            {
                Commanded* commanded = dynamic_cast<Commanded*>(childWindow[x]);
                if(commanded)
                    return commanded->InvokeAction(hwnd, uMsg, wParam, lParam);
            }
    }
    break;

    case WM_NOTIFY:
    {
        LPNMHDR item = (LPNMHDR)lParam;
        for (int x = 0; x < childWindow.size(); x++)
            if (item->code == NM_CUSTOMDRAW)
                if (item->idFrom == (UINT_PTR)childWindow[x]->hMenu)
                {
                    IHandleNotify* notify = dynamic_cast<IHandleNotify*>(childWindow[x]);
                    if(notify)
                        return notify->HandleNotify(hwnd, uMsg, wParam, lParam, item);
                }
    }
    break;

    case WM_CTLCOLORSTATIC:
    {
        for(int x = 0; x < childWindow.size(); x++)
            if (childWindow[x]->hwnd == (HWND)lParam)
            {
                IHandleNotifyStatic* notifyStatic = dynamic_cast<IHandleNotifyStatic*>(childWindow[x]);
                if (notifyStatic)
                    return notifyStatic->HandleNotifyStatic(hwnd, uMsg, wParam, lParam);
            }
    }
    break;

    case WM_CTLCOLORBTN:
    {
        return parent ? (LRESULT)parent->backgroundColor : (LRESULT)backgroundColor;
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (actionPaint.action != nullptr)
            actionPaint.action(this, ps, hdc, actionPaint.param);

        EndPaint(hwnd, &ps);
    }
    break;

    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void BaseWindow::AddWindow(BaseWindow* window)
{
    childWindow.push_back(window);
}

void BaseWindow::AddActionPaint(void* param, void(*action)(BaseWindow* window, PAINTSTRUCT ps, HDC hdc, void* param))
{
    actionPaint.param = param;
    actionPaint.action = action;
}

void BaseWindow::SetBackgroundColor(HBRUSH brush)
{
    this->backgroundColor = (HBRUSH)brush;
    InvalidateRect(hwnd, NULL, TRUE);
}

void BaseWindow::SetFont(HFONT font, COLORREF color)
{
    this->font = font;
    this->colorFont = color;
}

void BaseWindow::SetText(WCHAR* text)
{
    SendMessage(hwnd, WM_SETTEXT, NULL, (LPARAM)text);
}