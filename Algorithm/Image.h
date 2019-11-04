#ifndef IMAGE_H
#define IMAGE_H

#include "Core/Version.h"
#include "Core/Matrix.h"

namespace rapid
{

template<typename _Tp>
class Matrix;

struct RGBA;

using ImageSizeType = rapid::Matrix<RGBA>::SizeType;

extern Matrix<RGBA> filter(const Matrix<RGBA> &m, const Matrix<double> &f);
extern Matrix<RGBA> filter(const Matrix<RGBA> &m,
                           std::initializer_list<std::initializer_list<double>> f);
extern Matrix<RGBA> fft(const Matrix<RGBA> &m);
extern Matrix<RGBA> dft(const Matrix<RGBA> &m);

namespace magnification
{
extern Matrix<RGBA> nearest_neighbor_interpolation(const Matrix<RGBA> &m,
                                                   ImageSizeType new_row,
                                                   ImageSizeType new_column);
extern Matrix<RGBA> nearest_neighbor_interpolation(const Matrix<RGBA> &m,
                                                   double rate);
extern Matrix<RGBA> bilinear_interpolation(const Matrix<RGBA> &m,
                                           ImageSizeType new_row,
                                           ImageSizeType new_column);
extern Matrix<RGBA> bilinear_interpolation(const Matrix<RGBA> &m,
                                           double rate);
}

}

#endif // IMAGE_H
