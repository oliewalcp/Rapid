#include "Gui/Frame.h"
using namespace rapid;

int main()
{
    Frame *f = new Frame;
    f->set_string("测试窗口");
    f->set_size(300, 300);
    f->set_position(100, 100);
    return Application::loop();
}
