#include "Test/TestSingleLinkedList.h"
#include "Algorithm/Sorter.h"
#include <algorithm>

#ifdef QT_LIB
#include "Test/TreeTool.h"
#endif

using namespace rapid;

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

    std::sor

#ifdef EN_WIDGET
    delete app;
#endif
    return 0;
}
