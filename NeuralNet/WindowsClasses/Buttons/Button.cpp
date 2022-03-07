#include "Button.h"
#include <iostream>

Button::Button(
    LPCWSTR buttonText,
    HGDIOBJ brushIdentity,
    HGDIOBJ brushHot,
    HGDIOBJ brushSelected,
    BaseWindow* window, 
    HMENU hMenu)
	: Commanded(buttonText, BS_PUSHBUTTON | WS_VISIBLE, window->backgroundColor, window, hMenu)
{
    this->brushHot = brushHot;
    this->brushIdentity = brushIdentity;
    this->brushSelected = brushSelected;
	this->parent = window;
	this->toCreateNewClass = false;
}

LRESULT Button::HandleNotify(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LPNMHDR someItem)
{
        LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)someItem;

        if (item->uItemState & CDIS_SELECTED)
        {
            SelectObject(item->hdc, invisePen);
            SelectObject(item->hdc, brushSelected);
            RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);
        }
        else if (item->uItemState & CDIS_HOT)
        {
            SelectObject(item->hdc, invisePen);
            SelectObject(item->hdc, brushHot);
            RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);
        }
        else
        {
            //Select our color when our button is doing nothing
            SelectObject(item->hdc, invisePen);
            SelectObject(item->hdc, brushIdentity);
            RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 5, 5);
        }

        //Получение текста
        int len = GetWindowTextLength(someItem->hwndFrom);
        TCHAR* tchar = new TCHAR[len + 1];
        GetWindowText(someItem->hwndFrom, tchar, len + 1);

        //Отрисовка текста
        SetBkMode(item->hdc, TRANSPARENT); //Задаёт прозрачный фон текста
        if(font != NULL)
            SelectObject(item->hdc, font); //Выберает шрифт
        SetTextColor(item->hdc, colorFont); //Устанавливает цвет текста
        DrawText(item->hdc, tchar, len, &item->rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE); //Рисует текст

        delete[len + 1] tchar;

    return CDRF_NOTIFYPOSTPAINT;
}

const TCHAR* Button::ClassName()
{
	return L"Button";
}
