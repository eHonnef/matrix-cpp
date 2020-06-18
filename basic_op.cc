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
   * Matrix m must be square.
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
   * Matrix m must be square.
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
   * Gets the array matrix from LUP's return and make the pivot matrix.
   * Returns the pivot matrix.
   */
  static Matrix<int> make_pivot(const Matrix<int> &m) {
    Matrix<int> p(m.cols() - 1, m.cols() - 1);

    for (unsigned i = 0; i < p.cols(); ++i)
      p.at(i, m.at(0, i)) = 1;

    return p;
  }
  /*
   * Decomposes m into L and U using Crout's method with partial pivoting.
   * Matrices m and LU needs to be the same dimension.
   * It'll change the LU matrix param.
   * Returns the pivot vector, it is a 1x(N+1) matrix, where the element N+1 is the number of
   * row exchanges needed for determinant calculation.
   * Probably it'll give the wrong result if T isn't a double or float.
   */
  template <typename T> static Matrix<int> LUP_crout(const Matrix<T> &m, Matrix<T> &LU) {
    if (LU.rows() != m.rows() || LU.cols() != m.cols())
      throw std::domain_error("Number of rows and cols of LU must be equal to m");

    if (m.rows() != m.cols())
      throw std::domain_error("Matrix m must be square.");

    // assigning m to LU
    LU = m;
    // iterators
    unsigned i, j, k, r;
    // matrix dimension (for readibility)
    unsigned N = m.rows();
    // row index of max value
    unsigned i_max;
    // aux
    T max_val, abs_val, sum;
    // pivot array
    Matrix<int> pivot(1, N + 1);

    // filling pivot
    for (i = 0; i <= N; ++i)
      pivot.at(0, i) = i;

    for (k = 0; k < N; ++k) {
      max_val = 0;
      i_max = k;

      // checking if it's needed to pivot some row
      for (i = k; i < N; ++i) {
        if ((abs_val = fabs(LU.at(i, k))) > max_val) {
          max_val = abs_val;
          i_max = i;
        }
      }

      // swap rows
      if (i_max != k) {
        pivot.swap_cols(k, i_max);
        pivot.at(0, N) += 1;
        LU.swap_rows(k, i_max);
      }

      for (i = k; i < N; ++i) {
        sum = 0;
        for (r = 0; r < k; ++r)
          sum += LU.at(i, r) * LU.at(r, k);

        LU.at(i, k) = LU.at(i, k) - sum;
      }

      for (j = k + 1; j < N; ++j) {
        sum = 0;
        for (r = 0; r < k; ++r)
          sum += LU.at(k, r) * LU.at(r, j);

        LU.at(k, j) = (LU.at(k, j) - sum) / LU.at(k, k);
      }
    }

    return pivot;
  }
  /*
   * Splits the LU matrix (compact version) into L matrix and U matrix.
   * L and U need to be the same dimension of LU.
   * Params L and U will be modified.
   */
  template <typename T>
  static void split_LU_crout(const Matrix<T> &LU, Matrix<T> &L, Matrix<T> &U) {
    if (LU.rows() != LU.cols())
      throw std::domain_error("Matrix LU must be square.");

    unsigned i, j;
    for (i = 0; i < LU.rows(); ++i) {
      for (j = 0; j < LU.cols(); ++j) {
        if (i == j) {
          U.at(i, j) = 1;
          L.at(i, j) = LU.at(i, j);
        } else if (i > j) {
          U.at(i, j) = 0;
          L.at(i, j) = LU.at(i, j);
        } else {
          U.at(i, j) = LU.at(i, j);
          L.at(i, j) = 0;
        }
      }
    }
  }
  /*
   * Returns the determinant of the NxN matrix m using Crout's LU deecomposition with partial
   * pivoting.
   * Matrix m must be a square matrix.
   */
  template <typename T> static T determinant(const Matrix<T> &m) {
    if (m.rows() != m.cols())
      throw std::domain_error("Matrix m must be square.");

    Matrix<T> LU(m.rows(), m.cols());
    Matrix<int> p = LUP_crout(m, LU);
    T det = LU.at(0, 0);

    for (unsigned i = 1; i < LU.rows(); ++i)
      det *= LU.at(i, i);

    if ((p.at(0, LU.rows() - 1) - (LU.rows() - 1)) % 2 == 0)
      return det;
    else
      return -det;
  }
  /*
   * Returns the determinant of the NxN matrix given the matrix filled in LUP_crout.
   * Matrix LU is filled at LUP_crout.
   * Matrix pivot is the return of LUP_crout.
   */
  template <typename T> static T determinant_LU(const Matrix<T> &LU, const Matrix<int> &pivot) {
    if (LU.rows() != LU.cols())
      throw std::domain_error("Matrix m must be square.");

    T det = LU.at(0, 0);

    for (unsigned i = 1; i < LU.rows(); ++i)
      det *= LU.at(i, i);

    if ((pivot.at(0, LU.rows() - 1) - (LU.rows() - 1)) % 2 == 0)
      return det;
    else
      return -det;
  }
  /*
   * Returns the inverse matrix of m.
   * Matrix m must be square.
   */
  template <typename T> static Matrix<T> inverse(const Matrix<T> &m) {
    // @todo: working but need to fix the transposition
    // if (m.rows() != m.cols())
    //   throw std::domain_error("Matrix m must be square.");

    // Matrix<T> LU(m.rows(), m.cols());
    // Matrix<T> I(m.rows(), m.cols());
    // Matrix<int> p = LUP_crout(m, LU);

    // int i, j, k;
    // int N = m.rows();

    // for (j = 0; j < N; ++j) {
    //   for (i = 0; i < N; ++i) {
    //     if (p.at(0, i) == j)
    //       I.at(i, j) = 1;
    //     else
    //       I.at(i, j) = 0;

    //     for (k = 0; k < i; ++k)
    //       I.at(i, j) -= LU.at(i, k) * I.at(k, j);
    //   }

    //   for (i = N - 1; i >= 0; --i) {
    //     for (k = i + 1; k < N; ++k)
    //       I.at(i, j) -= LU.at(i, k) * I.at(k, j);

    //     I.at(i, j) = I.at(i, j) / LU.at(i, i);
    //   }
    // }

    // return I;
  }
  /*
   * Returns the result of the multiplication of m1 * m2.
   * The types T and U must be compatible.
   * It'll return a matrix of type T.
   */
  template <typename T, typename U>
  static Matrix<T> multiplication(const Matrix<T> &m1, const Matrix<U> &m2) {
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
   * Param dimension is (duh) the desired matrix dimension.
   */
  template <typename T> static Matrix<T> identity(unsigned dimension) {
    if (dimension == 0)
      throw std::invalid_argument("dimension can't be 0.");

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