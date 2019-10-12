#include "TestImage.h"
#include "Image/ImageBase.h"
#include <string>

static std::string path("C:\\Users\\Administrator\\Desktop\\");
static std::string file_name("test.bmp");

void rapid::test_Image_main()
{
    BMP b;
    b.parse((path + file_name).data());
    b.write((path + "result.bmp").data());
}
