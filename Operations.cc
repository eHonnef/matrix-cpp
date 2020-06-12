/***************************************************************************************************
 *
 ***************************************************************************************************/
#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include "Matrix.cc"

class M_OPERATION {
public:
  template <typename T> static Matrix<T> transpose(const Matrix<T> &m) {
    Matrix<T> rtn(m.cols(), m.rows());

    // #pragma omp parallel for
    for (unsigned n = 0; n < m.size(); n++) {
      // they are declared here for the omp parallel
      unsigned i = n / m.cols(); // row index
      unsigned j = n % m.cols(); // col index
      rtn.at(j, i) = m.at(i, j);
    }
    return rtn;
  }

  template <typename T> static T determinant(const Matrix<T> &m) {
    
  }

  template <typename T> static Matrix<T> u_triangular(const Matrix<T> &m) {
    if (m.rows() != m.cols())
      throw std::domain_error("The Row number must be equals to col number");

    Matrix<T> rtn(m.rows(), m.cols());
    for (unsigned i = 0; i < m.rows(); ++i)
      for (unsigned j = i; j < m.cols(); ++j)
        rtn.at(i, j) = m.at(i, j);

    return rtn;
  }

  template <typename T> static Matrix<T> l_triangular(const Matrix<T> &m) {
    if (m.rows() != m.cols())
      throw std::domain_error("The Row number must be equals to col number");

    Matrix<T> rtn(m.rows(), m.cols());
    for (unsigned i = 0; i < m.rows(); ++i)
      for (unsigned j = 0; j < (i + 1); ++j)
        rtn.at(i, j) = m.at(i, j);

    return rtn;
  }

  // static Matrix<T> inverse(const Matrix<T> &m) {}
  // static Matrix<T> multiplication(const Matrix<T> &m1, const Matrix<T> &m2) {}
  // static Matrix<T> multiplication(const Matrix<T> &m1, const T &value) {}
  // static Matrix<T> addition(const Matrix<T> &m1, const Matrix<T> &m2) {}
  // static Matrix<T> subtraction(const Matrix<T> &m1, const Matrix<T> &m2) {}
  // static T determinant(const Matrix<T> &m1) {}
};
#endif
/***************************************************************************************************
 *
 ***************************************************************************************************/