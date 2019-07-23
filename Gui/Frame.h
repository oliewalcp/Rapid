#ifndef FRAME_H
#define FRAME_H

#include "Widget.h"
#include "Application.h"

namespace rapid
{

class Frame : public WinWidget
{
protected:
    void _init(WinWidget *parent);
public:
    Frame(WinWidget *parent = nullptr)
    { _init(parent); }
    ~Frame();

    virtual void set_title(const char *title);
};

};
#endif // FRAME_H
