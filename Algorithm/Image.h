#ifndef IMAGE_H
#define IMAGE_H

#include "Core/Version.h"
#include "Core/Matrix.h"

namespace rapid
{

template<typename _Tp>
class Matrix;

struct RGBA;

namespace magnification
{
using SizeType = rapid::Matrix<RGBA>::SizeType;

Matrix<RGBA> nearest_neighbor_interpolation(const Matrix<RGBA> &m,
                                            SizeType new_row,
                                            SizeType new_column);
Matrix<RGBA> nearest_neighbor_interpolation(const Matrix<RGBA> &m,
                                            double rate);
Matrix<RGBA> bilinear_interpolation(const Matrix<RGBA> &m,
                                    SizeType new_row,
                                    SizeType new_column);
Matrix<RGBA> bilinear_interpolation(const Matrix<RGBA> &m,
                                    double rate);
}

}

#endif // IMAGE_H
