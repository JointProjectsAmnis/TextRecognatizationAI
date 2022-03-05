#pragma once
#include "BaseWindow.h"

class Commanded : public BaseWindow
{
public:

    struct Action
    {
        void* param;
        void(*action)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param);
    };

    Action action = { 0 };

	Commanded(
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        HBRUSH backgroundColor = WHITE_BRUSH,
        BaseWindow* parent = nullptr,
        HMENU hMenu = AUTO_CODE,
        DWORD dwExStyle = NULL);

    void AddAction(void* param, void(*action)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void* param));
    LRESULT InvokeAction(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

