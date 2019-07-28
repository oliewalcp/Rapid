#include "Frame.h"
#include "Application.h"

void rapid::Frame::__init(WinWidget *parent)
{
    if(parent == nullptr)
    {
        Application::create_control(this);
    }
    else
    {
        Application::create_control(this, "", parent);
    }
}

rapid::Frame::~Frame() { }
