#ifndef ITERATORBASE_H
#define ITERATORBASE_H

#define TreeIterator(type) \
class type \
{ \
private: \
    TreeNode *_M_current = nullptr; \
\
    void _F_next(); \
    void _F_previous(); \
\
    friend class BinaryTree; \
\
    type(const TreeNode *root); \
public: \
    type() { } \
    type(const type &it) : _M_current(it._M_current) { } \
    type(type &&it) \
        : _M_current(forward<type>(it)._M_current) \
    { } \
\
    type operator++() \
    { \
        type it = *this; \
        _F_next(); \
        return it; \
    } \
    type operator++(int) \
    { \
        _F_next(); \
        return *this; \
    } \
    type operator--() \
    { \
        type it = *this; \
        _F_previous(); \
        return it; \
    } \
    type operator--(int) \
    { \
        _F_previous(); \
        return *this; \
    } \
    DataType operator*() const \
    { return _M_current->data(); } \
    DataType* operator->() const \
    { return &_M_current->data(); } \
    bool operator==(const type &it) const \
    { return _M_current == it._M_current; } \
    bool operator==(type &&it) const \
    { return _M_current == forward<type>(it)._M_current; } \
    bool operator!=(const type &it) const \
    { return _M_current != it._M_current; } \
    bool operator!=(type &&it) const \
    { return _M_current != forward<type>(it)._M_current; } \
};


#endif // ITERATORBASE_H
