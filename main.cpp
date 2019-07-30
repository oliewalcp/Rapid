#include "Gui/Frame.h"
#include "Gui/Label.h"
#include "Gui/Application.h"
using namespace rapid;
#include <iostream>

void print_pos(WinWidget *wid)
{
    std::cout << "absolute x : " << wid->absolute_x() << std::endl;
    std::cout << "absolute y : " << wid->absolute_y() << std::endl;
    std::cout << "relative x : " << wid->relative_x() << std::endl;
    std::cout << "relative y : " << wid->relative_y() << std::endl;
    std::cout << "-------------" << std::endl;
}

int main()
{
    Frame *f = new Frame();
    f->set_title("测试");
    f->set_size(300, 300);
    f->set_position(100, 100);
    Label *label = new Label();
    label->set_text("呀哈哈哈1111");
    label->set_position(0, 0);
    label->set_size(200, 30);
    Label *label2 = new Label();
    label2->set_text("呀哈哈哈222");
    label2->set_position(0, 30);
    label2->set_size(200, 30);
    print_pos(f);
    print_pos(label);
    print_pos(label2);
    return Application::loop();
}
