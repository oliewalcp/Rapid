#ifndef FRAME_H
#define FRAME_H

#include "Widget.h"

namespace rapid
{

class Frame : public WinWidget
{
private:
    void __init(WinWidget *parent);
public:
    Frame(WinWidget *parent = nullptr)
    { __init(parent); }
    ~Frame();

    virtual void set_title(const char *title)
    { set_string(title); }
};

};
#endif // FRAME_H
