#ifndef TREE_H
#define TREE_H

namespace rapid
{

template<typename T>
struct Compare
{
    int operator()(const T &arg1, const T &arg2) const
    {
        if(arg1 < arg2) return 1;
        if(arg2 < arg1) return -1;
        return 0;
    }
};

}
#endif // TREE_H
