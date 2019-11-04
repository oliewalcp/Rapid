#include "Image.h"
#include "Core/Matrix.h"
#include "Image/RGB.h"

#define construct_result_matrix(new_row, new_column) \
    Matrix<RGBA> result(static_cast<ImageSizeType>(new_row), \
                        static_cast<ImageSizeType>(new_column)); \
    if(new_row < m.row() || new_column < m.column()) \
    { \
        return result; \
    }

#define foreach_result_matrix_begin \
    for(ImageSizeType i = 0; i < new_row; ++i) \
    { \
        for(ImageSizeType j = 0; j < new_column; ++j) \
        {

#define foreach_result_matrix_end \
        } \
    }

namespace rapid
{

static inline double distance(double src1, double src2)
{ return src1 > src2 ? src1 - src2 : src2 - src1; }
static inline double rate(double target, double src)
{ return distance(src, target); }

Matrix<RGBA> magnification::nearest_neighbor_interpolation(const Matrix<RGBA> &m,
                                                           ImageSizeType new_row,
                                                           ImageSizeType new_column)
{
    construct_result_matrix(new_row, new_column)
    double row_rate = static_cast<double>(new_row) / m.row();
    double column_rate = static_cast<double>(new_column) / m.column();
    foreach_result_matrix_begin
    result.set_value(i, j, m.get_value(static_cast<ImageSizeType>(i / row_rate),
                                       static_cast<ImageSizeType>(j / column_rate)));
    foreach_result_matrix_end
    return result;
}

Matrix<RGBA> magnification::nearest_neighbor_interpolation(const Matrix<RGBA> &m, double rate)
{ return nearest_neighbor_interpolation(m, static_cast<ImageSizeType>(m.row() * rate),
                                        static_cast<ImageSizeType>(m.column() * rate)); }

Matrix<RGBA> magnification::bilinear_interpolation(const Matrix<RGBA> &m,
                                                   ImageSizeType new_row,
                                                   ImageSizeType new_column)
{
    construct_result_matrix(new_row, new_column)
    double row_rate = static_cast<double>(new_row) / m.row();
    double column_rate = static_cast<double>(new_column) / m.column();
    foreach_result_matrix_begin
    double target_row = i / row_rate;
    double target_column = j / column_rate;
    ImageSizeType temp_row = static_cast<ImageSizeType>(target_row);
    ImageSizeType temp_column = static_cast<ImageSizeType>(target_column);
    RGBA rgba1 = m.get_value(temp_row, temp_column) * rate(temp_column, target_column) +
            m.get_value(temp_row, temp_column + 1) * rate(temp_column + 1, target_column);
    RGBA rgba2;
    if(temp_row >= m.row() - 1 || temp_column >= m.column() - 1)
    {
        rgba2 = rgba1;
    }
    else
    {
        rgba2 = m.get_value(temp_row + 1, temp_column) * rate(temp_column, target_column) +
                m.get_value(temp_row + 1, temp_column + 1) * rate(temp_column + 1, target_column);
    }
    result.set_value(i, j, rgba1 * rate(temp_row, target_row) + rgba2 * rate(temp_row + 1, target_row));
    foreach_result_matrix_end
    return result;
}

Matrix<RGBA> magnification::bilinear_interpolation(const Matrix<RGBA> &m, double rate)
{ return bilinear_interpolation(m, static_cast<ImageSizeType>(m.row() * rate),
                                static_cast<ImageSizeType>(m.column() * rate)); }

Matrix<RGBA> filter(const Matrix<RGBA> &m, const Matrix<double> &f)
{
    ImageSizeType center_row = (f.row() - 1) / 2, center_column = (f.column() - 1) / 2;

    Matrix<RGBA> result(m.row(), m.column());
    for(ImageSizeType i = 0; i < m.row(); i++)
    {
        for(ImageSizeType j = 0; j < m.column(); j++)
        {
            typename Matrix<RGBA>::DataType dt;
            for(ImageSizeType x = 0; x < f.row(); x++)
            {
                for(ImageSizeType y = 0; y < f.column(); y++)
                {
                    ImageSizeType tempx = i - center_row + x, tempy = j - center_column + y;
                    if(tempx < 0 || tempy < 0 || tempx >= m.row() || tempy >= m.column())
                        continue;
                    dt.construct(dt.content() + m.get_value(tempx, tempy) * f.get_value(x, y));
                }
            }
            result.set_value(i, j, dt.ref_content());
        }
    }
    return result;
}

Matrix<RGBA> filter(const Matrix<RGBA> &m, std::initializer_list<std::initializer_list<double> > f)
{ return filter(m, Matrix<double>(f)); }

Matrix<RGBA> fft(const Matrix<RGBA> &m)
{

}

Matrix<RGBA> dft(const Matrix<RGBA> &m)
{

}



} // end rapid
