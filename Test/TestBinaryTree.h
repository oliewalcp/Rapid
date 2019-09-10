#ifndef TESTBINARYTREE_H
#define TESTBINARYTREE_H

#include "Core/BinaryTree.h"
#include "Core/DoubleLinkedList.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <functional>

#define WORD_WIDTH 5

namespace rapid
{

template<typename T>
std::string to_string(const T &arg)
{
    std::string result, temp = std::to_string(arg);
    std::size_t s = result.size();
    std::size_t left = (WORD_WIDTH - s) / 2;
    std::size_t right = WORD_WIDTH - left;
    for(std::size_t i = 0; i < left; ++i)
    {
        result.push_back(' ');
    }
    result += temp;
    for(std::size_t i = 0; i < right; ++i)
    {
        result.push_back(' ');
    }
    return result;
}

void print_space(unsigned long size);

void print_element(size_type pos, const std::string &arg)
{
    print_space((pos - 1) * WORD_WIDTH);
    std::cout << arg;
}

template<typename T>
void print_tree(const BinaryTree<T> &tree)
{
    using BT = BinaryTree<T>;
    using TreeNode = typename BT::TreeNode;
    size_type total_element = static_cast<size_type>(pow(2, tree.depth()));
    DoubleLinkedList<TreeNode *> s;
    s.push_back(tree.root());
    for(size_type i = 1; i <= tree.depth(); ++i)
    {
        TreeNode *t = s.front();
        s.pop_front();
        s.push_back(BT::left_child(t));
        s.push_back(BT::right_child(t));
    }
}

void test_BinaryTree_main();
}

#endif // TESTBINARYTREE_H
