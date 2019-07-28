#include "Label.h"
#include "Application.h"

void rapid::Label::__init(rapid::WinWidget *parent, const char *text)
{
    Application::create_control(this, WS_VISIBLE, TEXT("Static"), parent);
    set_string(text);
}

rapid::Label::~Label() { }
