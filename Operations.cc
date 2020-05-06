/***************************************************************************************************
 *
 ***************************************************************************************************/
#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

template <typename T> class M_OPERATION {
public:
  static Matrix<T> transpose(const Matrix<T> &m) {
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
  static Matrix<T> inverse(const Matrix<T> &m) {}
  static Matrix<T> multiplication(const Matrix<T> &m1, const Matrix<T> &m2) {}
  static Matrix<T> multiplication(const Matrix<T> &m1, const T &value) {}
  static Matrix<T> addition(const Matrix<T> &m1, const Matrix<T> &m2) {}
  static Matrix<T> subtraction(const Matrix<T> &m1, const Matrix<T> &m2) {}
  static Matrix<T> determinant(const Matrix<T> &m1) {}
};
#endif
/***************************************************************************************************
 *
 ***************************************************************************************************/