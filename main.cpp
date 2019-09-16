//#include "Test/TestRedBlackTree.h"
#include "Test/TestAVLTree.h"
using namespace rapid;
#include <iostream>

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

    test_AVLTree_main();

#ifdef EN_WIDGET
    delete app;
#endif
    return 0;
}
