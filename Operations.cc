/***************************************************************************************************
 *
 ***************************************************************************************************/
#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include "Matrix.cc"
#include <cmath>

class M_OPERATION {
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
   * Decomposes m to the matrices L, U and P.
   * L, U, P must be the same dimension.
   * It'll change the L, U and P matrices.
   */
  template <typename T>
  static void LUP_decompose(const Matrix<T> &m, Matrix<T> &L, Matrix<T> &U, Matrix<T> &P) {
    // not very efficient
    if (m.rows() != m.cols())
      throw std::domain_error("The Row number must be equals to col number");

    unsigned N = m.rows(); // for readability
    P = pivot(m);
    Matrix<T> A = multiplication(P, m);

    unsigned i, j, k;
    for (j = 0; j < N; ++j) {
      L.at(j, j) = 1;
      for (i = 0; i <= j; ++i) {
        U.at(i, j) = A.at(i, j);
        for (k = 0; k < i; ++k)
          U.at(i, j) -= U.at(k, j) * L.at(i, k);
      }
      for (i = j; i < N; ++i) {
        L.at(i, j) = A.at(i, j);
        for (k = 0; k < j; ++k)
          L.at(i, j) -= U.at(k, j) * L.at(i, k);
        L.at(i, j) /= U.at(j, j);
      }
    }
  }

  /*
   * Matrix LU is changed, it contains a copy of both matrices L-E and U as A=(L-E)+U such that
   * P*A=L*U.
   * It returns the P matrix.
   * It was adapted from https://en.wikipedia.org/wiki/LU_decomposition
   */
  template <typename T> static Matrix<unsigned> LUP_compact(const Matrix<T> &m, Matrix<T> &LU) {
    // @todo: find a better algorithm

    if (m.rows() != m.cols())
      throw std::domain_error("The Row number must be equals to col number");

    unsigned i, j, k, iMax;
    unsigned N = m.rows();

    double maxA, absA;
    double Tol = 0.01;

    Matrix<unsigned> P(N + 1, 1);
    LU = m;

    for (auto i = N + 1; i-- > 0;)
      P.at(i, 0) = i;

    for (i = 0; i < N; ++i) {
      maxA = 0;
      iMax = i;

      for (k = i; k < N; ++k)
        if ((absA = fabs(LU.at(k, i))) > maxA) {
          maxA = absA;
          iMax = k;
        }

      if (maxA < Tol)
        throw std::runtime_error("Failure, matrix is degenerate");

      if (iMax != i) {
        // pivoting P
        P.swap_rows(i, iMax);

        // pivoting A
        LU.swap_rows(i, iMax);

        // counting pivots (for determinant)
        P.at(N, 0)++;
      }

      for (j = i + 1; j < N; ++j) {
        LU.at(j, i) /= LU.at(i, i);

        for (k = i + 1; k < N; ++k)
          LU.at(j, k) -= LU.at(j, i) * LU.at(i, k);
      }
    }

    return P;
  }
  /*
   * Returns the determinant of the NxN matrix m.
   * It uses the LU decomposition to calculate the determinant.
   * Adapted from wikipedia:LU_decomposition.
   * keep in mind that T should be a double, you'll get the wrong result if it is an integer.
   */
  template <typename T> static T determinant(const Matrix<T> &m) {
    // @todo: fix this after fixing the compact LU
    if (m.rows() != m.cols())
      throw std::domain_error("The Row number must be equals to col number");

    Matrix<T> LU(m.rows(), m.cols());
    Matrix<unsigned> P = LUP_compact(m, LU);
    unsigned N = m.rows();
    T det = LU.at(0, 0);

    for (unsigned i = 1; i < N; ++i)
      det *= LU.at(i, i);

    if ((P.at(N, 0) - N) % 2 == 0)
      return det;
    else
      return -det;
  }
  /*
   * Returns the inverse matrix of m.
   */
  template <typename T> static Matrix<T> inverse(const Matrix<T> &m) {
    //@todo: it's wrong
    // if (m.rows() != m.cols())
    //   throw std::domain_error("The Row number must be equals to col number");

    // Matrix<T> LU(m.rows(), m.cols());
    // Matrix<unsigned> P = LUP_compact(m, LU);
    // Matrix<T> rtn(m.rows(), m.cols());

    // int i, j, k;
    // int N = m.rows();

    // for (j = 0; j < N; ++j) {
    //   for (i = 0; i < N; ++i) {
    //     if (P.at(i, 0) == (unsigned) j)
    //       rtn.at(i, j) = 1;
    //     else
    //       rtn.at(i, j) = 0;

    //     for (k = 0; k < i; ++k)
    //       rtn.at(i, j) -= LU.at(i, k) * LU.at(k, j);
    //   }

    //   for (i = N - 1; i >= 0; --i) {
    //     for (k = i + 1; k < N; ++k)
    //       rtn.at(i, j) -= LU.at(i, k) * LU.at(k, j);

    //     rtn.at(i, j) = rtn.at(i, j) / LU.at(i, i);
    //   }
    // }

    // return rtn;
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

    Matrix<T> rtn(rows, cols, T(0));
    for (auto i = rows; i-- > 0;)
      rtn.at(i, i) = 1;

    return rtn;
  }

  // static Matrix<T> multiplication(const Matrix<T> &m1, const T &value) {}
  // static Matrix<T> addition(const Matrix<T> &m1, const Matrix<T> &m2) {}
  // static Matrix<T> subtraction(const Matrix<T> &m1, const Matrix<T> &m2) {}
  // static T determinant(const Matrix<T> &m1) {}
private:
  /*
   * Returns the pivot matrix of m.
   */
  template <typename T> static Matrix<T> pivot(const Matrix<T> &m) {
    unsigned N = m.rows();
    unsigned P[N];
    for (auto i = N; i-- > 0;)
      P[i] = i;

    for (unsigned col = 0; col < N; ++col) {
      unsigned maxI = col;
      for (unsigned row = col; row < N; ++row) {
        if (m.at(row, col) > m.at(maxI, col))
          maxI = row;
      }
      if (col != maxI)
        std::swap(P[col], P[maxI]);
    }

    Matrix<T> rtn(N, N);
    for (auto i = N; i-- > 0;)
      rtn.at(i, P[i]) = 1;

    return rtn;
  }
};
#endif
/***************************************************************************************************
 *
 ***************************************************************************************************/