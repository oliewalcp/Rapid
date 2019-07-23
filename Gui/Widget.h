#ifndef WINCONTROL_H
#define WINCONTROL_H

#include "GuiConfig.h"

namespace rapid
{
class ButtonEvent
{
public:
    virtual ~ButtonEvent();
    virtual void click_event() { }
    virtual void double_click_event() { }
    virtual void mouse_down_event() { }
    virtual void mouse_up_event() { }
    virtual void mouse_over_event() { }
    virtual void mouse_out_event() { }
    virtual void mouse_move_event() { }
    virtual void mouse_enter_event() { }
    virtual void mouse_leave_event() { }
};

class InputEvent : public ButtonEvent
{
public:
    virtual ~InputEvent();
    virtual void edit_event() { }
};

class WinWidget
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
};

};
#endif // WINCONTROL_H


