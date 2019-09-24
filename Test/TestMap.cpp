#include "TestMap.h"
#include "Core/Map.h"
#include <iostream>

void rapid::test_AVLMap_main()
{
    AVLMap<int, std::string> avl;
    avl[1] = "abd";
    avl.insert(20, "123435");
    std::cout << avl[1] << std::endl;
    std::cout << avl[20] << std::endl;
    avl.insert(1, "----****----");
    std::cout << avl.find(1)->Second << std::endl;
    std::cout << (*avl.find(1)).Second << std::endl;
    Pair<int, std::string> p;
    p.First = 100;
    p.Second = "|||||||||||||||||";
    avl.insert(p);
    avl.insert(Pair<int, std::string>(-100, "test"));
    std::cout << avl[100] << std::endl;
    std::cout << avl[-100] << std::endl;
}

void rapid::test_Map_main()
{
    Map<int, std::string> rb_map;
    rb_map[1] = "abd";
    rb_map.insert(20, "123435");
    std::cout << rb_map[1] << std::endl;
    std::cout << rb_map[20] << std::endl;
    rb_map.insert(1, "----****----");
    std::cout << rb_map.find(1)->Second << std::endl;
    std::cout << (*rb_map.find(1)).Second << std::endl;
    Pair<int, std::string> p;
    p.First = 100;
    p.Second = "|||||||||||||||||";
    rb_map.insert(p);
    rb_map.insert(Pair<int, std::string>(-100, "test"));
    std::cout << rb_map[100] << std::endl;
    std::cout << rb_map[-100] << std::endl;
}
