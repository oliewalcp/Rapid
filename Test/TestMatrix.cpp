#include "TestMatrix.h"
#include "Core/Exception.h"
#include "Core/Matrix.h"
#include <iostream>
#include <iomanip>
using SizeType = long;

template<typename T>
static void print_matrix(rapid::Matrix<T> &m)
{
    std::cout << std::setiosflags(std::ios::left);
    for(SizeType i = 0; i < m.row(); i++)
    {
        for(SizeType j = 0; j < m.column(); j++)
        {
            std::cout << std::setw(4) << m.get_value(i, j);
        }
        std::cout << std::endl;
    }
    std::cout << std::resetiosflags(std::ios::showbase);
}

void rapid::test_Matrix_main()
{
    std::cout << "-----------test Matrix start----------" << std::endl;
    Matrix<int> m1(5, 5, 2);
    print_matrix(m1);
    std::cout << "---------------------" << std::endl;
    m1.set_value(0, 1, 0);
    m1.set_value(1, 2, 0);
    m1.set_value(2, 3, 0);
    m1.set_value(3, 4, 0);
    print_matrix(m1);
    std::cout << "---------------------" << std::endl;
    Matrix<int> m2(3, 4, 3);
    Matrix<int> m3(m1);
    try
    {
        [[maybe_unused]] Matrix<int> *m4 = Matrix<int>::multiply(m2, m3);
    }
    catch(Exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    m3.multiply(m1);
    print_matrix(m3);
    std::cout << "---------------------" << std::endl;
    m2.T();
    print_matrix(m2);
    std::cout << "---------------------" << std::endl;
    m1.power(2);
    print_matrix(m1);
    std::cout << "---------------------" << std::endl;
    Matrix<int> m5(4, 3, 2);
    m2.T();
    m5 *= m2;
    print_matrix(m5);
    std::cout << "---------------------" << std::endl;
    Matrix<int> m6({
                       {2, 3, 7, 10},
                       {1, 2, 10, 1},
                       {5, 5, 5, 5},
                       {6, 6, 6, 6},
                       {1, 1, 3, 3},
                       {22, -2, -10, -5},
                       {1, 5, 5, 5},
                   });
    Matrix<int> m7({
                    {1, -1},
                    {-1, 1}
                });
    m6.filter(m7);
    print_matrix(m6);
    std::cout << "-----------test Matrix end----------" << std::endl;
    m1.clear();
    m2.clear();
    m3.clear();
    m5.clear();
    m6.clear();
    m7.clear();
}
