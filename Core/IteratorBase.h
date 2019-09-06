#ifndef ITERATORBASE_H
#define ITERATORBASE_H

#include "TypeTraits.h"

namespace rapid
{

template<typename ImplType, typename DataType, typename NodeType, typename FriendType = void>
class SingleIteratorBase : protected ImplType
{
protected:
    friend FriendType;
    SingleIteratorBase(NodeType *n) : ImplType(n) { }
public:
    SingleIteratorBase() : ImplType(nullptr) { }
    SingleIteratorBase(const SingleIteratorBase &it) : ImplType(it) { }
    SingleIteratorBase(SingleIteratorBase && it) : ImplType(forward<SingleIteratorBase>(it)) { }

    SingleIteratorBase operator++()
    {
        SingleIteratorBase it = *this;
        ImplType::_F_next();
        return it;
    }
    SingleIteratorBase operator++(int)
    {
        ImplType::_F_next();
        return *this;
    }
    DataType operator*() const
    { return ImplType::_M_current->data(); }
    DataType* operator->() const
    { return ImplType::_M_current->address(); }
    bool operator==(const SingleIteratorBase &it) const
    { return ImplType::_M_current == it._M_current; }
    bool operator==(SingleIteratorBase &&it) const
    { return ImplType::_M_current == forward<SingleIteratorBase>(it)._M_current; }
    bool operator!=(const SingleIteratorBase &it) const
    { return ImplType::_M_current != it._M_current; }
    bool operator!=(SingleIteratorBase &&it) const
    { return ImplType::_M_current != forward<SingleIteratorBase>(it)._M_current; }
    SingleIteratorBase operator=(const SingleIteratorBase &it)
    { return ImplType::_M_current = it._M_current; }
};

template<typename ImplType, typename DataType, typename NodeType, typename FriendType = void>
class BothIteratorBase : public ImplType
{
protected:
    friend FriendType;
    BothIteratorBase(NodeType *n) : ImplType(n) { }
public:
    BothIteratorBase() : ImplType(nullptr) { }
    BothIteratorBase(const BothIteratorBase &it) : ImplType(it) { }
    BothIteratorBase(BothIteratorBase && it) : ImplType(forward<BothIteratorBase>(it)) { }

    BothIteratorBase operator++()
    {
        BothIteratorBase it = *this;
        ImplType::_F_next();
        return it;
    }
    BothIteratorBase operator++(int)
    {
        ImplType::_F_next();
        return *this;
    }
    BothIteratorBase operator--()
    {
        BothIteratorBase it = *this;
        ImplType::_F_previous();
        return it;
    }
    BothIteratorBase operator--(int)
    {
        ImplType::_F_previous();
        return *this;
    }
    DataType operator*() const
    { return ImplType::_M_current->data(); }
    DataType* operator->() const
    { return ImplType::_M_current->address(); }
    bool operator==(const BothIteratorBase &it) const
    { return ImplType::_M_current == it._M_current; }
    bool operator==(BothIteratorBase &&it) const
    { return ImplType::_M_current == forward<BothIteratorBase>(it)._M_current; }
    bool operator!=(const BothIteratorBase &it) const
    { return ImplType::_M_current != it._M_current; }
    bool operator!=(BothIteratorBase &&it) const
    { return ImplType::_M_current != forward<BothIteratorBase>(it)._M_current; }
    BothIteratorBase operator=(const BothIteratorBase &it)
    { return ImplType::_M_current = it._M_current; }
};

}

#endif // ITERATORBASE_H
