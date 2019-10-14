#include "TestImage.h"
#include "Image/ImageBase.h"
#include "Core/Matrix.h"
#include "Image/rgb.h"
#include <string>
#include <iostream>

static std::string path("C:\\Users\\Administrator\\Desktop\\");
static std::string file_name("test.bmp");

void rapid::test_BMP_main()
{
    std::cout << "--------" << __func__ << " start---------" << std::endl;
    BMP b;
    b.parse((path + file_name).data());
    Matrix<RGB> m = b.to_matrix();
    std::cout << "exchange finish" << std::endl;
    BMP bb;
    bb.parse(m);
    bb.write((path + "result.bmp").data());
    std::cout << "test end" << std::endl;
}
