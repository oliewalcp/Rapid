//#include "Image/ImageBase.h"
#include "Gui/Frame.h"
using namespace rapid;

int main()
{
    Application *a = Application::application();
    Frame f;
    f.set_title("测试");
    f.set_size(300, 300);
    f.set_position(100, 100);
    return a->exec();
}
