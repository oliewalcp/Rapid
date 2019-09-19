#include "Test/TestMap.h"

#ifdef QT_LIB
#include "Test/TreeTool.h"
using namespace rapid;
#endif

#ifdef cpp17
#if __has_include(<QApplication>)
    #define EN_WIDGET
#endif
#endif

int main(int argc, char *argv[])
{
#ifdef EN_WIDGET
    app = new QApplication(argc, argv);
#endif

//    test_AVLMap_main();
    test_Map_main();

#ifdef EN_WIDGET
    delete app;
#endif
    return 0;
}
