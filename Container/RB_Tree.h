#ifndef RB_TREE_H
#define RB_TREE_H
#include "Stack.h"
#define BLACK 0
#define RED   1
using Color = bool;

#define isNull(p) ((p) == nullptr)

template<typename _key, typename _value>
struct RBTreeNode
{
    RBTreeNode(RBTreeNode<_key, _value> *parent = nullptr, RBTreeNode<_key, _value> *left = nullptr, RBTreeNode<_key, _value> *right = nullptr)
    {
        init(parent, left, right);
    }
    RBTreeNode(_key key, _value value, Color color, RBTreeNode<_key, _value> *parent = nullptr, RBTreeNode<_key, _value> *left = nullptr, RBTreeNode<_key, _value> *right = nullptr)
    {
        init(parent, left, right);
        this->color = color;
        this->key = key;
        this->value = value;
    }
    inline void init(RBTreeNode<_key, _value> *parent, RBTreeNode<_key, _value> *left, RBTreeNode<_key, _value> *right)
    {
        this->parent = parent;
        this->left = left;
        this->right = right;
        color = RED;
        left_number = 0;
        right_number = 0;
    }
    bool operator<(const _key &arg) const
    {
        return this->key < arg;
    }
    RBTreeNode<_key, _value> *left;//左孩子结点
    RBTreeNode<_key, _value> *right;//右孩子结点
    RBTreeNode<_key, _value> *parent;//父结点
    unsigned int left_number;//左子树结点数
    unsigned int right_number;//右子树结点数
    _key key;//键
    _value value;//值
    Color color;//颜色
};

//红黑树结构
template<typename _key, typename _value = _key>
class RB_Tree
{
private:
    RBTreeNode<_key, _value> *root = nullptr;//根节点
    unsigned int nodeNumber = 0;
    /* 左转
     * y是x的右孩子结点，x = node
     * 将x设置为y的左孩子结点
     * 将y的左孩子设置为x的右孩子
     * param[node]:旋转的支点结点
     */
    void L_Rotate(RBTreeNode<_key, _value> *node);
    /* 右转
     * y是x的左孩子结点，x = node
     * 将x设置为y的右孩子结点
     * 将y的右孩子设置为x的左孩子
     * param[node]:旋转的支点结点
     */
    void R_Rotate(RBTreeNode<_key, _value> *node);
    /* 追溯添加结点数量
     * param[node]:新插入的结点
     */
    void add_number(RBTreeNode<_key, _value> *node);
    //插入调整
    /*RB-INSERT-FIXUP(T, z)
while color[p[z]] = RED                                                  // 若“当前节点(z)的父节点是红色”，则进行以下处理。
    do if p[z] = left[p[p[z]]]                                           // 若“z的父节点”是“z的祖父节点的左孩子”，则进行以下处理。
          then y ← right[p[p[z]]]                                        // 将y设置为“z的叔叔节点(z的祖父节点的右孩子)”
               if color[y] = RED                                         // Case 1条件：叔叔是红色
                  then color[p[z]] ← BLACK                    ▹ Case 1   //  (01) 将“父节点”设为黑色。
                       color[y] ← BLACK                       ▹ Case 1   //  (02) 将“叔叔节点”设为黑色。
                       color[p[p[z]]] ← RED                   ▹ Case 1   //  (03) 将“祖父节点”设为“红色”。
                       z ← p[p[z]]                            ▹ Case 1   //  (04) 将“祖父节点”设为“当前节点”(红色节点)
                  else if z = right[p[z]]                                // Case 2条件：叔叔是黑色，且当前节点是右孩子
                          then z ← p[z]                       ▹ Case 2   //  (01) 将“父节点”作为“新的当前节点”。
                               LEFT-ROTATE(T, z)              ▹ Case 2   //  (02) 以“新的当前节点”为支点进行左旋。
                          color[p[z]] ← BLACK                 ▹ Case 3   // Case 3条件：叔叔是黑色，且当前节点是左孩子。
                                                                         //  (01) 将“父节点”设为“黑色”。
                          color[p[p[z]]] ← RED                ▹ Case 3   //  (02) 将“祖父节点”设为“红色”。
                          RIGHT-ROTATE(T, p[p[z]])            ▹ Case 3   //  (03) 以“祖父节点”为支点进行右旋。
       else (same as then clause with "right" and "left" exchanged)      // 若“z的父节点”是“z的祖父节点的右孩子”，将上面的操作中“right”和“left”交换位置，然后依次执行。
color[root[T]] ← BLACK
     */
    void insert_fixed(RBTreeNode<_key, _value> *node);

    inline void uncle_red_fixed(RBTreeNode<_key, _value> *node, RBTreeNode<_key, _value> *uncle)
    {
        node->parent->color = BLACK;
        uncle->color = BLACK;
        uncle->parent->color = RED;
    }
    inline void current_left(RBTreeNode<_key, _value> *node, RBTreeNode<_key, _value> *uncle)
    {
        node->parent->color = BLACK;
        uncle->parent->color = RED;
    }

public:
    void insert_node(_key key, _value value);//插入数据

    //前序遍历迭代器
    class former_sequence_iterator final
    {
    private:
        RBTreeNode<_key, _value> *current = nullptr;
        RBTreeNode<_key, _value> *next = nullptr;
        friend class RB_Tree;
        //获取某根结点下最左叶子节点
        RBTreeNode<_key, _value> * get_left_leaf(RBTreeNode<_key, _value> *node)
        {
            RBTreeNode<_key, _value> *result = node;
            while(result->left != nullptr)
                result = result->left;
            return result;
        }
        void next_node()
        {
            if(next == nullptr) current = nullptr;
            else
            {
                RBTreeNode<_key, _value> *temp = next;
                //如果下一个节点是当前结点的左孩子
                if(next == current->left)
                    next = next->left != nullptr ? next->left : current->right;
                //如果下一个节点是当前结点的兄弟结点
                else if(next == current->parent->right)
                    next = current->parent->parent->right;
                //如果下一个节点是当前结点的叔叔结点
                else if(next == current->parent->right)
                    next = next->left != nullptr ? next->left : current->right;
                current = temp;
            }
        }
    public:
        former_sequence_iterator(RBTreeNode<_key, _value> *p = nullptr, RBTreeNode<_key, _value> *pn = nullptr) : current(p), next(pn){}
        former_sequence_iterator& operator++() {
            next_node();
            return *this;
        }
        former_sequence_iterator operator++(int) {
            next_node();
            return former_sequence_iterator(current, next);
        }
        _key first(){return current->key;}
        _value second(){return current->value;}
        template<typename T>
        bool operator==(const T &arg) const {
            return arg.current == this->current;
        }
        template<typename T>
        bool operator!=(const T &arg) const {
            return arg.current != this->current;
        }
    };
    //中序遍历迭代器
    class middle_sequence_iterator final
    {
    private:
        RBTreeNode<_key, _value> *current = nullptr;
        RBTreeNode<_key, _value> *next = nullptr;
        friend class RB_Tree;
        //获取某根结点下最左叶子节点
        RBTreeNode<_key, _value> * get_left_leaf(RBTreeNode<_key, _value> *node)
        {
            RBTreeNode<_key, _value> *result = node;
            while(result->left != nullptr)
                result = result->left;
            return result;
        }
        void next_node()
        {
            if(next == nullptr) current = nullptr;
            else
            {
                RBTreeNode<_key, _value> *temp = next;
                //如果下一个节点是当前结点的父结点
                if(next == current->parent)
                    next = next->right;
                //如果下一个节点是当前结点的右孩子
                else if(next == current->right)
                    next = current->parent;
                //如果下一个节点是当前结点的祖父结点
                else if(next == current->parent->parent)
                    next = next->right;
                current = temp;
            }
        }
    public:
        middle_sequence_iterator(RBTreeNode<_key, _value> *p = nullptr, RBTreeNode<_key, _value> *pn = nullptr) : current(p), next(pn){}
        middle_sequence_iterator& operator++() {
            next_node();
            return *this;
        }
        middle_sequence_iterator operator++(int) {
            next_node();
            return middle_sequence_iterator(current, next);
        }
        _key first(){return current->key;}
        _value second(){return current->value;}
        bool operator==(const middle_sequence_iterator &arg) const {
            return arg.current == this->current;
        }
        bool operator!=(const middle_sequence_iterator &arg) const {
            return arg.current != this->current;
        }
    };
    //后序遍历迭代器
    class after_sequence_iterator final
    {
    private:
        RBTreeNode<_key, _value> *current = nullptr;
        RBTreeNode<_key, _value> *next = nullptr;
        friend class RB_Tree;
        //获取某根结点下最左叶子节点
        RBTreeNode<_key, _value> * get_left_leaf(RBTreeNode<_key, _value> *node)
        {
            RBTreeNode<_key, _value> *result = node;
            while(result->left != nullptr)
                result = result->left;
            return result;
        }
        void next_node()
        {
            if(next == nullptr) current = nullptr;
            else
            {
                RBTreeNode<_key, _value> *temp = next;
                //如果下一个节点是当前结点的兄弟结点
                if(next == current->parent->right)
                    next = current->parent;
                //如果下一个节点是右孩子
                else if(next == next->parent->right)
                    next = next->parent;
                //如果下一个节点是当前结点的父结点
                else if(next == current->parent)
                    next = get_left_leaf(next->parent->right);
                current = temp;
            }
        }
    public:
        after_sequence_iterator(RBTreeNode<_key, _value> *p = nullptr, RBTreeNode<_key, _value> *pn = nullptr) : current(p), next(pn){}
        after_sequence_iterator& operator++() {
            next_node();
            return *this;
        }
        after_sequence_iterator operator++(int) {
            next_node();
            return after_sequence_iterator(current, next);
        }
        _key first(){return current->key;}
        _value second(){return current->value;}
        bool operator==(const after_sequence_iterator &arg) const {
            return arg.current == this->current;
        }
        bool operator!=(const after_sequence_iterator &arg) const {
            return arg.current != this->current;
        }
    };

    using fiterator = former_sequence_iterator;
    using miterator = middle_sequence_iterator;
    using aiterator = after_sequence_iterator;
    using iterator = middle_sequence_iterator;//默认为中序遍历

    fiterator fbegin(){return fiterator(root, root->left);}
    fiterator fend() {return fiterator();}

    miterator mbegin()
    {
        middle_sequence_iterator it;
        it.current = it.get_left_leaf(root);
        it.next = it.current->parent;
        return it;
    }
    miterator mend() {return miterator();}

    aiterator abegin()
    {
        middle_sequence_iterator it;
        it.current = it.get_left_leaf(root);
        it.next = it.current->parent != nullptr ? it.current->parent->right : nullptr;
        return it;
    }
    aiterator aend() {return aiterator();}

    miterator begin() {return mbegin();}
    miterator end() {return miterator();}
};
template<typename _key, typename _value>
void RB_Tree<_key, _value>::insert_node(_key key, _value value)
{
    RBTreeNode<_key, _value> *node, *temp = root;
    if(temp == nullptr)
    {
        root = new RBTreeNode<_key, _value>(key, value, BLACK);
        nodeNumber++;
        return;
    }
    while(temp != nullptr)
    {
        if(key < temp->key)
        {
            if(temp->left == nullptr)
            {
                node = new RBTreeNode<_key, _value>(key, value, RED, temp);
                temp->left = node;
                break;
            }
            else temp = temp->left;
        }
        else if(temp->key < key)
        {
            if(temp->right == nullptr)
            {
                node = new RBTreeNode<_key, _value>(key, value, RED, temp);
                temp->right = node;
                break;
            }
            else temp = temp->right;
        }
        else
        {
            temp->value = value;
            return;
        }
    }
    add_number(node);
    insert_fixed(node);
}
template<typename _key, typename _value>
void RB_Tree<_key, _value>::insert_fixed(RBTreeNode<_key, _value> *node)
{
    //如果node结点的父节点是红色结点
    while(!isNull(node->parent) && !isNull(node->parent->parent) && node->parent->color == RED)
    {
        //如果node结点的父结点是node结点的祖父结点的左孩子
        if(node->parent == node->parent->parent->left)
        {
            RBTreeNode<_key, _value> *uncle = node->parent->parent->right;//获取叔叔结点
            //如果node结点的叔叔结点是红色结点
            if(!isNull(uncle) && uncle->color == RED)
            {
                uncle_red_fixed(node, uncle);
                node = uncle->parent;
            }
            //如果node结点是其父结点的右孩子
            else if(node == node->parent->right)
            {
                node = node->parent;
                L_Rotate(node);//左旋操作
            }
            //如果node结点是其父结点的左孩子
            else
            {
                current_left(node, uncle);
                R_Rotate(uncle->parent);//右旋操作
            }
        }
        else
        {
            RBTreeNode<_key, _value> *uncle = node->parent->parent->left;//获取叔叔结点
            //如果node结点的叔叔结点是红色结点
            if(!isNull(uncle) && uncle->color == RED)
            {
                uncle_red_fixed(node, uncle);
                node = node->parent->parent;
            }
            //如果node结点是其父结点的右孩子
            else if(node == node->parent->left)
                R_Rotate(node->parent);//左旋操作
            //如果node结点是其父结点的左孩子
            else
            {
                current_left(node, uncle);
                L_Rotate(node->parent->parent);//右旋操作
            }
        }
    }
    root->color = BLACK;
}
template<typename _key, typename _value>
void RB_Tree<_key, _value>::add_number(RBTreeNode<_key, _value> *node)
{
    RBTreeNode<_key, _value> *temp = node->parent;
    while(temp != nullptr)
    {
        if(node == temp->left)
            temp->left_number++;
        else
            temp->right_number++;
        node = temp;
        temp = temp->parent;
    }
    nodeNumber++;
}
template<typename _key, typename _value>
void RB_Tree<_key, _value>::L_Rotate(RBTreeNode<_key, _value> *node)
{
    if(node == nullptr || node->right == nullptr) return;
    RBTreeNode<_key, _value> *rightChild = node->right;
    node->right = rightChild->left;//将y的左孩子设置为x的右孩子
    rightChild->left = node;//将x设置为y的左孩子结点
    if(node != root)
    {
        if(node->parent->left == node) node->parent->left = rightChild;
        else node->parent->right = rightChild;
    }
    else root = rightChild;
    node->parent = rightChild;//将y设置为x的父节点
    rightChild->left_number = node->left_number + node->right_number;
    if(isNull(node->right))
        node->right_number = 0;
    else
        node->right_number = node->right->left_number + node->right->right_number;
}

template<typename _key, typename _value>
void RB_Tree<_key, _value>::R_Rotate(RBTreeNode<_key, _value> *node)
{
    if(node == nullptr || node->left == nullptr) return;
    RBTreeNode<_key, _value> *leftChild = node->left;
    node->left = leftChild->right;//将y的右孩子设置为x的左孩子
    leftChild->right = node;//将x设置为y的右孩子结点
    if(node != root)
    {
        if(node->parent->left == node) node->parent->left = leftChild;
        else node->parent->right = leftChild;
    }
    else root = leftChild;
    node->parent = leftChild;//将y设置为x的父节点
    if(isNull(leftChild->left))
        leftChild->left_number = 0;
    else
        leftChild->left_number = leftChild->left->left_number + leftChild->left->right_number;
    node->right_number = leftChild->left_number + leftChild->right_number;
}
#endif // RB_TREE_H
