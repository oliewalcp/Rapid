#ifndef WINCONTROL_H
#define WINCONTROL_H

#include "GuiConfig.h"

namespace rapid
{

class EventInterface
{
public:
    virtual ~EventInterface();
};

class ButtonEvent : public EventInterface
{
public:
    virtual ~ButtonEvent();
    virtual void double_click_event() { }
    virtual void left_click_event() { }
    virtual void left_down_event() { }
    virtual void left_up_event() { }
    virtual void right_click_event() { }
    virtual void right_down_event() { }
    virtual void right_up_event() { }
    virtual void middle_click_event() { }
    virtual void middle_up_event() { }
    virtual void middle_down_event() { }
};

class MouseEvent : public EventInterface
{
public:
    virtual ~MouseEvent();
    virtual void mouse_wheel_event() { }
    virtual void mouse_move_event() { }
    virtual void mouse_over_event() { }
    virtual void mouse_out_event() { }
    virtual void mouse_enter_event() { }
    virtual void mouse_leave_event() { }
};

class InputEvent : public EventInterface
{
public:
    virtual ~InputEvent();
    virtual void edit_event() { }
};

class KeyBoardEvent : public EventInterface
{
public:
    virtual ~KeyBoardEvent();
    virtual void key_down() { }
    virtual void key_up() { }
};

class WinWidget : public MouseEvent
{
protected:
    HWND _Hwnd;
    WinWidget() { }
public:
    virtual ~WinWidget();
    virtual void repaint_event() { }
    virtual void close_event() { }
    virtual void update();
    virtual void show();
    virtual void hide();
    virtual void set_size(WinSizeType w, WinSizeType h);
    virtual void set_position(WinSizeType x, WinSizeType y);
    virtual void set_string(const char* str);
    virtual void set_enable(bool ena);
    virtual WinSizeType width();
    virtual WinSizeType height();
    virtual WinSizeType relative_x();
    virtual WinSizeType relative_y();
    virtual WinSizeType absolute_x();
    virtual WinSizeType absolute_y();
    virtual void set_parent(WinWidget *widget)
    { SetParent(_Hwnd, widget->_Hwnd); }
    virtual WinWidget* get_parent();
};

};
#endif // WINCONTROL_H


