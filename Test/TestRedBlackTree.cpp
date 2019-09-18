#include "TestRedBlackTree.h"
#include "TreeTool.h"

void rapid::test_RedBlackTree_main()
{
    std::cout << "------------" << __func__ << "------------" << std::endl;
    RedBlackTree<int> rb_tree;
    rb_tree.insert({50, 45, 40, 48, 39, 43, 47, 49, 38, 42, 44, 46});
#ifdef QT_LIB
    auto get_data_func = [](const BTreeNode<RBDataNode<int>> *node) -> std::string {
        if(node == nullptr) return "";
        return std::to_string(node->data().data());
    };
    auto get_color_func = [](const BTreeNode<RBDataNode<int>> *node) -> unsigned long {
        return static_cast<unsigned long>(node->data().color());
    };
    MainWindow<RBDataNode<int>, RedBlackTree<int>> mw0(rb_tree.to_ordinary_tree(),
                                    get_data_func, get_color_func);
    mw0.setWindowTitle("half");
    mw0.show();
#endif
    rb_tree.insert({100, 90, 200, 60, 95, 199, 202, 80, 94, 96, 201, 300});
    std::cout << "former iterator" << std::endl;
    for(auto it = rb_tree.fbegin(); it != rb_tree.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : rb_tree)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = rb_tree.abegin(); it != rb_tree.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
#ifdef QT_LIB
    MainWindow<RBDataNode<int>, RedBlackTree<int>> mw1(rb_tree.to_ordinary_tree(),
                                    get_data_func, get_color_func);
    mw1.setWindowTitle("ordinary");
    mw1.show();
#endif
    std::cout << "---------------erase-------------" << std::endl;
    rb_tree.erase(rb_tree.find(100));
    std::cout << "former iterator" << std::endl;
    for(auto it = rb_tree.fbegin(); it != rb_tree.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : rb_tree)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = rb_tree.abegin(); it != rb_tree.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "---------------------------------" << std::endl;
#ifdef QT_LIB
    MainWindow<RBDataNode<int>, RedBlackTree<int>> mw3(rb_tree.to_ordinary_tree(), get_data_func, get_color_func);
    mw3.setWindowTitle("erase 100");
    mw3.show();
#endif
    rb_tree.erase(rb_tree.find(48));
    std::cout << "former iterator" << std::endl;
    for(auto it = rb_tree.fbegin(); it != rb_tree.fend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "middle iterator" << std::endl;
    for(int i : rb_tree)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "after iterator" << std::endl;
    for(auto it = rb_tree.abegin(); it != rb_tree.aend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    std::cout << "------------end------------" << std::endl;
#ifdef QT_LIB
    MainWindow<RBDataNode<int>, RedBlackTree<int>> mw2(rb_tree.to_ordinary_tree(), get_data_func, get_color_func);
    mw2.setWindowTitle("erase 48");
    mw2.show();
#endif
#ifdef QT_LIB
    app->exec();
#endif
}
