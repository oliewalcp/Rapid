#ifndef LABEL_H
#define LABEL_H

#include "Widget.h"

namespace rapid
{

class Label : public WinWidget
{
private:
    void __init(WinWidget *parent, const char *text);
public:
    Label(WinWidget *parent = nullptr, const char *text = "")
    { __init(parent, text); }
    Label(const char *text)
    { __init(nullptr, text); }

    void set_text(const char *text)
    { set_string(text); }

    virtual ~Label();
};

}

#endif // LABEL_H
