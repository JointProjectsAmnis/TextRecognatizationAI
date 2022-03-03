#pragma once
#include <windows.h>
#include <vector>
#include <CommCtrl.h>

#define AUTO_CODE (HMENU)(-1)

class BaseWindow
{
public:
	HWND hwnd;
    LPCWSTR lpWindowName;
    DWORD dwStyle;
    HWND hWndParent;
    HMENU hMenu;
    DWORD dwExStyle;
    HBRUSH backgroundColor;
    bool toCreateNewClass = true;

    HFONT font = NULL;
    COLORREF colorFont = RGB(0, 0, 0);

    static HMENU hMenuCounter;

    std::vector<BaseWindow*> daughtWindows = std::vector<BaseWindow*>();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	BaseWindow(
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        HBRUSH backgroundColor = WHITE_BRUSH,
        HWND hWndParent = NULL,
        HMENU hMenu = AUTO_CODE,
        DWORD dwExStyle = NULL);

	void Create(
        int X = CW_USEDEFAULT,
        int Y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT);

    void Show(int nCmdShow);
    void AddWindow(BaseWindow* window);
    void SetFont(HFONT font, COLORREF color);

protected:
	virtual LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT HandleNotify(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LPNMHDR someItem);
	virtual const TCHAR* ClassName() = 0;
};

