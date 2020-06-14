/***************************************************************************************************
 *
 ***************************************************************************************************/
#ifndef MATRIX_BASIC_OPERATIONS_H
#define MATRIX_BASIC_OPERATIONS_H

#include "Matrix.cc"
#include <cmath>

class basic_op {
public:
  /*
   * Returns the transpose of m.
   */
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
  /*
   * Returns the upper triangular matrix of m.
   */
  template <typename T> static Matrix<T> u_triangular(const Matrix<T> &m) {
    if (m.rows() != m.cols())
      throw std::domain_error("The Row number must be equals to col number");

    Matrix<T> rtn(m.rows(), m.cols());
    for (unsigned i = 0; i < m.rows(); ++i)
      for (unsigned j = i; j < m.cols(); ++j)
        rtn.at(i, j) = m.at(i, j);

    return rtn;
  }
  /*
   * Returns the lower triangular matrix of m.
   */
  template <typename T> static Matrix<T> l_triangular(const Matrix<T> &m) {
    if (m.rows() != m.cols())
      throw std::domain_error("The Row number must be equals to col number");

    Matrix<T> rtn(m.rows(), m.cols());
    for (unsigned i = 0; i < m.rows(); ++i)
      for (unsigned j = 0; j < (i + 1); ++j)
        rtn.at(i, j) = m.at(i, j);

    return rtn;
  }
  /*
   * Decomposes m into L and U using Cholesky's method. It'll only works in a positive-definite
   * symmetrical matrix.
   * It'll change the LU matrix.
   */
  template <typename T> static void LU_cholesky(const Matrix<T> &m, Matrix<T> &LU) {
    //@todo
  }
  /*
   * Decomposes m into L and U using Crout's method with partial pivoting.
   * It'll change the LU matrix.
   * Returns the pivot matrix.
   */
  template <typename T> static Matrix<unsigned> LUP_crout(const Matrix<T> &m, Matrix<T> &LU) {
    // @todo
  }
  /*
   * Decomposes m into L and U using Crout's method with full pivoting
   * It'll change the LU matrix.
   * Returns the pivot matrix.
   */
  template <typename T> static Matrix<unsigned> LUF_crout(const Matrix<T> &m, Matrix<T> &LU) {
    // @todo
  }
  /*
   * Returns the determinant of the NxN matrix m.
   */
  template <typename T> static T determinant(const Matrix<T> &m) {
    // @todo
  }
  /*
   * Returns the inverse matrix of m.
   */
  template <typename T> static Matrix<T> inverse(const Matrix<T> &m) {
    //@todo: it's wrong
  }
  /*
   * Returns the result of the multiplication of m1 * m2
   */
  template <typename T> static Matrix<T> multiplication(const Matrix<T> &m1, const Matrix<T> &m2) {
    // @todo
    // naive method for now
    if (m1.cols() != m2.rows())
      throw std::invalid_argument("Number of cols must be equal to rows.");

    Matrix<T> rtn(m1.rows(), m2.cols());
    for (unsigned i = 0; i < rtn.rows(); ++i)
      for (unsigned j = 0; j < rtn.cols(); ++j)
        for (unsigned k = 0; k < m1.cols(); ++k)
          rtn.at(i, j) += (m1.at(i, k) * m2.at(k, j));

    return rtn;
  }
  /*
   * Returns an identity matrix.
   */
  template <typename T> static Matrix<T> identity(unsigned dimension) {
    if (dimension > 0)
      throw std::invalid_argument("Can't be 0.");

    Matrix<T> rtn(dimension, dimension, T(0));
    for (auto i = dimension; i-- > 0;)
      rtn.at(i, i) = 1;

    return rtn;
  }

private:
};
#endif
/***************************************************************************************************
 *
 ***************************************************************************************************/