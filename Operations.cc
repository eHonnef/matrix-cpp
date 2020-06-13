/***************************************************************************************************
 *
 ***************************************************************************************************/
#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include "Matrix.cc"
#include <cmath>

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

  template <typename T>
  static void LUP_decomposition(const Matrix<T> &m, Matrix<T> &L, Matrix<T> &U, Matrix<T> &P) {
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

  template <typename T> static T determinant(const Matrix<T> &m) {
    // adapted from wikipedia:LU_decomposition#C_code_examples
    // keep in mind that T should be a double, you'll get the wrong result if it is an integer.
    if (m.rows() != m.cols())
      throw std::domain_error("The Row number must be equals to col number");

    unsigned i, j, k, iMax;
    unsigned N = m.rows();
    unsigned P[N + 1];
    double maxA, absA;
    double Tol = 0.01;
    Matrix<T> A = m;

    for (auto i = N + 1; i-- > 0;)
      P[i] = i;

    for (i = 0; i < N; ++i) {
      maxA = 0;
      iMax = i;

      for (k = i; k < N; ++k)
        if ((absA = fabs(A.at(k, i))) > maxA) {
          maxA = absA;
          iMax = k;
        }

      if (maxA < Tol)
        throw std::runtime_error("Failure, matrix is degenerate");

      if (iMax != i) {
        // pivoting P
        j = P[i];
        P[i] = P[iMax];
        P[iMax] = j;

        // pivoting A
        A.swap_rows(i, iMax);

        // counting pivots (for determinant)
        P[N]++;
      }

      for (j = i + 1; j < N; ++j) {
        A.at(j, i) /= A.at(i, i);

        for (k = i + 1; k < N; ++k)
          A.at(j, k) -= A.at(j, i) * A.at(i, k);
      }
    }

    T det = A.at(0, 0);

    for (i = 1; i < N; ++i)
      det *= A.at(i, i);

    if ((P[N] - N) % 2 == 0)
      return det;
    else
      return -det;
  }

  // static Matrix<T> inverse(const Matrix<T> &m) {}
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
  // static Matrix<T> multiplication(const Matrix<T> &m1, const T &value) {}
  // static Matrix<T> addition(const Matrix<T> &m1, const Matrix<T> &m2) {}
  // static Matrix<T> subtraction(const Matrix<T> &m1, const Matrix<T> &m2) {}
  // static T determinant(const Matrix<T> &m1) {}
private:
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