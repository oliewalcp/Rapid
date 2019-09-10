#include "TestBinaryTree.h"
#include "Core/DoubleLinkedList.h"


void rapid::print_space(unsigned long size)
{
    for(unsigned long i = 0; i < size; ++i)
    {
        std::cout << ' ';
    }
}

void rapid::test_BinaryTree_main()
{
    std::cout << "------------" << __func__ << "------------" << std::endl;
    using Node = BinaryTree<int>::TreeNode;
    BinaryTree<int> bt;
    Node *not_need;
    Node *root = bt.append_root(50);
    Node *temp = bt.append_left(root, 10);
    bt.append_left(temp, 20);
    temp = bt.append_right(temp, 100);
    not_need = temp = bt.append_right(root, 500);
    bt.append_right(temp, -1);
    temp = bt.append_left(temp, -200);
    temp = bt.append_left(temp, 1);
    temp = bt.append_left(temp, 2);
    std::cout << "size = " << bt.size() << std::endl;
    print_tree(bt);
//    std::cout << "middle: " << std::endl;
//    for(int i : bt)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "former: " << std::endl;
//    for(auto it = bt.fbegin(); it != bt.fend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt.abegin(); it != bt.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "depth: ----->  " << bt.depth() << std::endl;
//    std::cout << std::endl << "----------------------------" << std::endl;
//    BinaryTree<int> bt2(bt);
//    std::cout << "size = " << bt2.size() << std::endl;
//    std::cout << "middle: " << std::endl;
//    for(int i : bt2)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "former: " << std::endl;
//    for(auto it = bt2.fbegin(); it != bt2.fend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt2.abegin(); it != bt2.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << std::endl << "----------------------------" << std::endl;
//    bt.right_rotate(root);
//    std::cout << "size = " << bt.size() << std::endl;
//    for(int i : bt)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt.abegin(); it != bt.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    bt.left_rotate(root);
//    bt.left_rotate(bt.root());
//    std::cout << std::endl;
//    std::cout << "size = " << bt.size() << std::endl;
//    for(int i : bt)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt.abegin(); it != bt.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl << "------------reverse----------------" << std::endl;
//    std::cout << "former reverse" << std::endl;
//    for(auto it = bt2.frbegin(); it != bt2.frend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "middle reverse" << std::endl;
//    for(auto it = bt2.mrbegin(); it != bt2.mrend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "after reverse" << std::endl;
//    for(auto it = bt2.arbegin(); it != bt2.arend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "--------------erase--------------" << std::endl;
//    bt.erase(not_need);
//    std::cout << "size = " << bt.size() << std::endl;
//    std::cout << "middle: " << std::endl;
//    for(int i : bt)
//    {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl << "former: " << std::endl;
//    for(auto it = bt.fbegin(); it != bt.fend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl << "after: " << std::endl;
//    for(auto it = bt.abegin(); it != bt.aend(); ++it)
//    {
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
}

