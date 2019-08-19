#include "Widget.h"
#include "Application.h"
#include "Core/Conver.h"

rapid::EventInterface::~EventInterface() { }
rapid::MouseEvent::~MouseEvent() { }
rapid::ButtonEvent::~ButtonEvent() { }
rapid::WinWidget::~WinWidget() { }
rapid::InputEvent::~InputEvent() { }
rapid::KeyBoardEvent::~KeyBoardEvent() { }

WinSizeType rapid::WinWidget::width()
{
    RECT r;
    GetWindowRect(_Hwnd, &r);
    return r.right - r.left;
}

WinSizeType rapid::WinWidget::height()
{
    RECT r;
    GetWindowRect(_Hwnd, &r);
    return r.bottom - r.top;
}

WinSizeType rapid::WinWidget::absolute_x()
{
    RECT r;
    GetWindowRect(_Hwnd, &r);
//    ClientToScreen(_Hwnd, reinterpret_cast<LPPOINT>(&r));
    return r.left;
}

WinSizeType rapid::WinWidget::absolute_y()
{
    RECT r;
    GetWindowRect(_Hwnd, &r);
//    ClientToScreen(_Hwnd, reinterpret_cast<LPPOINT>(&r));
    return r.top;
}

rapid::WinWidget *rapid::WinWidget::get_parent()
{
    return Application::__get_widget(GetParent(_Hwnd));
}

void rapid::WinWidget::set_string(const char *str)
{
//    SizeType len = strlen(str) + 1;
//    WinStringType *txt = new WinStringType[len];
//    to_win_code(txt, str, len);
//    SetWindowText(_Hwnd, txt);
    SetWindowText(_Hwnd, to_utf8(str));
}

void rapid::WinWidget::set_enable(bool ena)
{
    EnableWindow(_Hwnd, ena);
}

WinSizeType rapid::WinWidget::relative_x()
{
    RECT r, parent_r;
    HWND parent = GetParent(_Hwnd);
    GetWindowRect(_Hwnd, &r);
    // success
    if(GetWindowRect(parent, &parent_r) != 0)
    {
        return r.left - parent_r.left;
    }
    // failure
    else
    {
        return r.left;
    }
}

WinSizeType rapid::WinWidget::relative_y()
{
    RECT r, parent_r;
    HWND parent = GetParent(_Hwnd);
    GetWindowRect(_Hwnd, &r);
    // success
    if(GetWindowRect(parent, &parent_r) != 0)
    {
        return r.top - parent_r.top - 25;
    }
    // failure
    else
    {
        return r.top;
    }
}

void rapid::WinWidget::set_size(WinSizeType w, WinSizeType h)
{
    RECT r;
    GetWindowRect(_Hwnd, &r);
    MoveWindow(_Hwnd, r.left, r.top, w, h, true);
}

void rapid::WinWidget::set_position(WinSizeType x, WinSizeType y)
{
    WinWidget *parent = get_parent();
    RECT r;
    GetWindowRect(_Hwnd, &r);
    if(parent != nullptr)
    {
        MoveWindow(_Hwnd, x + parent->absolute_x()/* - 206*/, y + parent->absolute_y()/* - 225*/, r.right - r.left, r.bottom - r.top, true);
    }
    else
    {
        MoveWindow(_Hwnd, x, y, r.right - r.left, r.bottom - r.top, true);
    }
}

void rapid::WinWidget::show()
{
    ShowWindow(_Hwnd, SW_HIDE);
}

void rapid::WinWidget::hide()
{
    ShowWindow(_Hwnd, SW_SHOW);
}

void rapid::WinWidget::update()
{
    UpdateWindow(_Hwnd);
}
