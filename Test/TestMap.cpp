#include "TestMap.h"
#include <iostream>

void rapid::test_AVLMap_main()
{
    AVLMap<int, std::string> avl;
    avl[1] = "abd";
    std::cout << avl[1] << std::endl;
}

void rapid::test_Map_main()
{

}
