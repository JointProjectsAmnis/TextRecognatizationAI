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
    BaseWindow* parent;
    HMENU hMenu;
    DWORD dwExStyle;
    HBRUSH backgroundColor;
    bool toCreateNewClass = false;

    HFONT font = NULL;
    COLORREF colorFont = RGB(0, 0, 0);

    static HMENU hMenuCounter;
    static HPEN invisePen;

    std::vector<BaseWindow*> childWindow = std::vector<BaseWindow*>();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	BaseWindow(
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        HBRUSH backgroundColor = WHITE_BRUSH,
        BaseWindow* parent = nullptr,
        HMENU hMenu = AUTO_CODE,
        DWORD dwExStyle = NULL);

	void Create(
        int X = CW_USEDEFAULT,
        int Y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT);

    void Show(int nCmdShow);

    //Добавление окна
    void AddWindow(BaseWindow* window);

    //Настройка внешнего вида окна
    void SetFont(HFONT font, COLORREF color);
    void SetBackgroundColor(HBRUSH brush);

protected:
	virtual LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual const TCHAR* ClassName() = 0;
};

