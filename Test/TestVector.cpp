#include "TestCase.h"
#include "Core/Exception.h"
#include "Core/Vector.h"

template<typename T>
static void print_vector(rapid::Vector<T> &v)
{
    for(T value : v)
        std::cout << value << std::endl;
}
void rapid::test_Vector_main()
{
    std::cout << "-----------test_Vector_main-----------" << std::endl;
    Vector<long> v;
    v.push_back(10);
    v.push_back(30);
    print_vector(v);
    v.push_back(50);
    v.push_back(70);
    print_vector(v);
    v.push_back(20);
    v.push_back(40);
    std::cout << "size = " << v.size() << std::endl;
    print_vector(v);
    std::cout << "------------------------------" << std::endl;
    v.erase(v.find(10));
    v.pop_back();
    v.pop_front();
    std::cout << "size = " << v.size() << std::endl;
    print_vector(v);
    std::cout << "------------------------------" << std::endl;
    v.erase(v.find(10));
    v.push_back(100);
    v.push_back(200);
    v.push_front(0);
    v.insert(v.find(100), 90);
    std::cout << "size = " << v.size() << std::endl;
    print_vector(v);
    std::cout << "------------------------------" << std::endl;
    Vector<long> vec(v);
    std::cout << "size = " << vec.size() << std::endl;
    std::cout << "front: " << vec.front() << std::endl;
    std::cout << "back: " << vec.back() << std::endl;
    try
    {
        std::cout << "the element at index 3 is: " << vec.at(3) << std::endl;
        std::cout << "the element at index 10 is: " << vec.at(10) << std::endl;
    }
    catch (IndexOutOfArrayException e)
    {
        std::cout << e.what() << std::endl;
    }
    std::cout << "size = " << vec.size() << std::endl;
    print_vector(vec);
    std::cout << "---------------test end---------------" << std::endl;
}
