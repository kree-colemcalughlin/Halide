#ifndef HALIDE_TYPE_H
#define HALIDE_TYPE_H

#include <assert.h>
#include <stdint.h>

/** \file
 * Defines a simple matrix type for use in Halide expresions.
 */

namespace Halide {

template<typename T, int NumRows, int NumCols>
struct Mat {
    typedef T Scalar;

    static const int num_rows = NumRows;
    static const int num_cols = NumCols;
    static const int num_elem = num_rows * num_cols;

    Mat<Scalar, num_cols, num_rows> transpose() const {
        Mat<Scalar, num_cols, num_rows> At;
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                At(j, i) = data[i*num_rows + j];
            }
        }
    }

    Mat& operator*= (const Scalar& alpha) {
        for (int i = 0; i < num_elem; ++i) {
            data[i] *= alpha;
        }
    }

    Mat& operator+= (const Mat& other) {
        assert(num_rows == other.num_rows);
        assert(num_cols == other.num_cols);
        for (int i = 0; i < num_elem; ++i) {
            data[i] += other.data[i];
        }
    }

    Mat& operator-= (const Mat& other) {
        assert(num_rows == other.num_rows);
        assert(num_cols == other.num_cols);
        for (int i = 0; i < num_elem; ++i) {
            data[i] -= other.data[i];
        }
    }

    Scalar& operator() (int i, int j) {
        assert(0 < i && i < num_rows);
        assert(0 < j && j < num_cols);
        return data[i*num_cols + j];
    }

    const Scalar& operator() (int i, int j) const {
        assert(0 < i && i < num_rows);
        assert(0 < j && j < num_cols);
        return data[i*num_cols + j];
    }

    Scalar& operator[] (int i) {
        assert(0 < i && i < num_elem);
        return data[i];
    }

    const Scalar& operator[] (int i) const {
        assert(0 < i && i < num_elem);
        return data[i];
    }

    Scalar data[num_elem];
};

template<typename T, int N1, int N2, int M>
Mat<T,N1,N2> operator* (const Mat<T,N1,M>& A, const Mat<T,M,N2>& B) {
    Mat<T,N1,N2> AB;

    for (int i = 0; i < N1; ++i) {
        for (int j = 0; j < N2; ++j) {
            AB(i, j) = 0;

            for (int k = 0; k < M; ++k) {
                AB(i, j) += A(i, k) * B(k, j);
            }
        }
    }

    return AB;
}

}

#endif
