#ifndef MATRIX_H
#define MATRIX_H

#include <exception>
#include <ostream>
#include <sstream>
#include <string>
#include <typeinfo>

template <class T> class Matrix {
public:
  Matrix(unsigned rows, unsigned cols, T init_val = T()) {
    _rows = rows;
    _cols = cols;
    alloc();

    for (auto i = _rows; i-- > 0;)
      for (auto j = _cols; j-- > 0;)
        _matrix[i][j] = init_val;
  }

  Matrix(const std::initializer_list<std::initializer_list<T>> &init) {
    _rows = init.size();
    _cols = init.begin()->size();
    alloc();

    unsigned row = 0;
    unsigned col = 0;
    for (const auto &it_r : init) {
      if (it_r.size() != _cols)
        throw std::runtime_error("The row <" + std::to_string(row) +
                                 "> has more/less columns than the others.");
      for (const auto &it_c : it_r)
        _matrix[row][col++] = it_c;

      col = 0;
      row += 1;
    }
  }

  ~Matrix() {}

  std::string to_string() const {
    std::ostringstream ss;
    ss << *this;
    return ss.str();
  }

  unsigned rows() const { return _rows; }
  unsigned cols() const { return _cols; }
  unsigned size() const { return _rows * _cols; }

  T *operator[](unsigned index) { return _matrix[index]; }
  T *operator[](unsigned index) const { return _matrix[index]; }
  Matrix &operator=(const Matrix &m) {}
  Matrix &operator+=(const Matrix &m) {}
  Matrix &operator-=(const Matrix &m) {}
  Matrix &operator*=(const Matrix &m) {}
  Matrix &operator*=(const T &value) {}
  Matrix &operator/=(const T &value) {}

  bool operator==(const Matrix &m) const {
    if (m.rows() != _rows)
      return false;
    else if (m.cols() != _cols)
      return false;

    for (auto i = _rows; i-- > 0;)
      for (auto j = _cols; j-- > 0;)
        if (m[i][j] != _matrix[i][j])
          return false;

    return true;
  }

  bool operator!=(const Matrix &m) const { return !(*this == m); }

  const Matrix &operator+(const Matrix &m) { return (*this += m); }
  const Matrix &operator-(const Matrix &m) { return (*this -= m); }
  const Matrix &operator*(const Matrix &m) { return (*this *= m); }

  Matrix transpose() {
    Matrix<T> rtn(_cols, _rows);
    for (auto i = _rows; i-- > 0;)
      for (auto j = _cols; j-- > 0;)
        rtn[j][i] = _matrix[i][j];

    return rtn;
  }

private:
  unsigned _rows;
  unsigned _cols;
  T **_matrix;

  void alloc() {
    _matrix = new T *[_rows];
    for (auto i = _rows; i-- > 0;)
      _matrix[i] = new T[_cols];
  }
};

template <class T> inline std::ostream &operator<<(std::ostream &outs, const Matrix<T> &m) {
  return outs << "Matrix<" << typeid(T).name() << ">[" << m.rows() << "][" << m.cols() << "]";
}

#endif // MATRIX_H
