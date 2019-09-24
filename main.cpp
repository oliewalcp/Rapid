#include "Core/DoubleLinkedList.h"
#include "Core/SingleLinkedList.h"
#include "Algorithm/Sorter.h"

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

    DoubleLinkedList<int> dll1;
    auto it = dll1.push_back(10);
    dll1.push_back(20);
    dll1.push_back(30);
    dll1.push_front(100);
    dll1.push_front(200);
    it = dll1.insert(it, 0);
    dll1.insert(it, -10);
    it = dll1.insert(it, -20);
    for(int i : dll1)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    fsort(dll1.begin(), dll1.end());
    for(int i : dll1)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

#ifdef EN_WIDGET
    delete app;
#endif
    return 0;
}
