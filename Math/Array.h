#ifndef ARRAY_H
#define ARRAY_H

struct Array
{
private:
    typedef unsigned int __uint32;
public:
    template <typename T, __uint32 __n>
    static __uint32 GetLength(T (&array)[__n])
    {
        return __n;
    }
    template <typename T, __uint32 _line_, __uint32 _column_>
    static __uint32 GetLength(T (&array)[_line_][_column_])
    {
        return _line_ * _column_;
    }
};
#endif // ARRAY_H
