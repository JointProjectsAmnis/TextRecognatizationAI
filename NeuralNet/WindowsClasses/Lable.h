#pragma once
#include "BaseWindow.h"
#include "IHandleNotifyStatic.h"

class Lable : public BaseWindow, public IHandleNotifyStatic
{
public:

    Lable(
        LPCWSTR text,
        COLORREF colorText,
        HFONT font,
        BaseWindow* parent = nullptr,
        HMENU hMenu = AUTO_CODE,
        DWORD dwExStyle = NULL);

    void SetText(WCHAR* text);

    virtual const TCHAR* ClassName();
    virtual LRESULT HandleNotifyStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

