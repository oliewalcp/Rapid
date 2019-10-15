#include "Image.h"
#include "Core/Matrix.h"
#include "Image/RGB.h"

namespace rapid
{

template<typename T>
static inline constexpr T avg(const T &t1, const T &t2)
{ return (t1 + t2) / 2; }

Matrix<RGBA> magnification::nearest_neighbor_interpolation(const Matrix<RGBA> &m,
                                                           Matrix<RGBA>::SizeType new_row,
                                                           Matrix<RGBA>::SizeType new_column)
{
    using SizeType = Matrix<RGBA>::SizeType;
    Matrix<RGBA> result(static_cast<SizeType>(new_row),
                        static_cast<SizeType>(new_column));
    if(new_row < m.row() || new_column < m.column())
    {
        return result;
    }
    double row_rate = static_cast<double>(new_row) / m.row();
    double column_rate = static_cast<double>(new_column) / m.column();
    for(SizeType i = 0; i < new_row; ++i)
    {
        for(SizeType j = 0; j < new_column; ++j)
        {
            result.set_value(i, j, m.get_value(static_cast<SizeType>(i / row_rate),
                                               static_cast<SizeType>(j / column_rate)));
        }
    }
    return result;
}

Matrix<RGBA> magnification::nearest_neighbor_interpolation(const Matrix<RGBA> &m, double rate)
{ return nearest_neighbor_interpolation(m, static_cast<Matrix<RGBA>::SizeType>(m.row() * rate),
                                        static_cast<Matrix<RGBA>::SizeType>(m.column() * rate)); }

Matrix<RGBA> magnification::bilinear_interpolation(const Matrix<RGBA> &m,
                                                   Matrix<RGBA>::SizeType new_row,
                                                   Matrix<RGBA>::SizeType new_column)
{
    Matrix<RGBA> result(static_cast<Matrix<RGBA>::SizeType>(new_row),
                        static_cast<Matrix<RGBA>::SizeType>(new_column));
    if(new_row < m.row() || new_column < m.column())
    {
        return result;
    }
    return result;
}

Matrix<RGBA> magnification::bilinear_interpolation(const Matrix<RGBA> &m, double rate)
{ return bilinear_interpolation(m, static_cast<Matrix<RGBA>::SizeType>(m.row() * rate),
                                static_cast<Matrix<RGBA>::SizeType>(m.column() * rate)); }



} // end rapid
