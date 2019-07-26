#include "Frame.h"

void rapid::Frame::_init(WinWidget *parent)
{
    WinStringType *t = new WinStringType[2]{0};
    to_win_code(t, "", 1);
    if(parent == nullptr)
        _Hwnd = Application::create_control(this);
}

rapid::Frame::~Frame() { }
