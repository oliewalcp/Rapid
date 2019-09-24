#include "TestSet.h"
#include "Core/Set.h"
#include "TreeTool.h"
#include <iostream>

void rapid::test_AVLSet_main()
{
    std::cout << "------------" << __func__ << "------------" << std::endl;
    AVLSet<int> avl;
    avl.insert({50, 45, 40, 48, 39, 43, 47, 49, 38, 42, 44, 46});
    avl.insert({100, 90, 200, 60, 95, 199, 202, 80, 94, 96, 201, 300});
    std::cout << "former iterator" << std::endl;
    for(auto it = avl.fbegin(); it != avl.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : avl)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = avl.abegin(); it != avl.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------erase-------------" << std::endl;
    avl.erase(avl.find(100));
    std::cout << "former iterator" << std::endl;
    for(auto it = avl.fbegin(); it != avl.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : avl)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = avl.abegin(); it != avl.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------------------------" << std::endl;
    avl.erase(avl.find(48));
    std::cout << "former iterator" << std::endl;
    for(auto it = avl.fbegin(); it != avl.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : avl)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = avl.abegin(); it != avl.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "------------end------------" << std::endl;
}

void rapid::test_Set_main()
{
    std::cout << "------------" << __func__ << "------------" << std::endl;
    Set<int> rbset;
    rbset.insert({50, 45, 40, 48, 39, 43, 47, 49, 38, 42, 44, 46});
    rbset.insert({100, 90, 200, 60, 95, 199, 202, 80, 94, 96, 201, 300});
    std::cout << "former iterator" << std::endl;
    for(auto it = rbset.fbegin(); it != rbset.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : rbset)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = rbset.abegin(); it != rbset.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------erase-------------" << std::endl;
    rbset.erase(rbset.find(100));
    std::cout << "former iterator" << std::endl;
    for(auto it = rbset.fbegin(); it != rbset.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : rbset)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = rbset.abegin(); it != rbset.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------------------------" << std::endl;
    rbset.erase(rbset.find(48));
    std::cout << "former iterator" << std::endl;
    for(auto it = rbset.fbegin(); it != rbset.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : rbset)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = rbset.abegin(); it != rbset.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "------------end------------" << std::endl;
}
