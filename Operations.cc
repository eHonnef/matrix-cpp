/***************************************************************************************************
 *
 ***************************************************************************************************/
#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H
template <typename T> class Matrix;
template <typename T> class M_OPERATION {
public:
  static Matrix<T> transpose(const Matrix<T> &m) {
    Matrix<T> rtn(m.cols(), m.rows());
    for (auto i = m.rows(); i-- > 0;)
      for (auto j = m.cols(); j-- > 0;)
        rtn.at(j, i) = m.at(i, j);

    return rtn;
  }
  // static Matrix<T> multiplication(const Matrix<T> &m) {}
};
#endif
/***************************************************************************************************
 *
 ***************************************************************************************************/