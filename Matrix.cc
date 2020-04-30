/***************************************************************************************************
 *
 ***************************************************************************************************/
#ifndef P_DELETE_H
#define P_DELETE_H
namespace Utils {
template <typename U> inline void pDelete(U *&pointer) {
  if (pointer) {
    delete pointer;
    pointer = nullptr;
  }
}
template <typename U> inline void pDelete(U pointer) { return; }
} // namespace Utils
#endif // P_DELETE_H

#ifndef MATRIX_H
#define MATRIX_H
/***************************************************************************************************
 *
 ***************************************************************************************************/
#include "Operations.cc"
#include <algorithm>
#include <cstring>
#include <exception>
#include <iomanip>
#include <iostream>
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

    fill(init_val);
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
        _array[index(row, col++)] = it_c;

      col = 0;
      row += 1;
    }
  }

  Matrix(unsigned rows, unsigned cols, const T *array) {
    _rows = rows;
    _cols = cols;
    alloc();

    std::memmove(_array, array, rows * cols * sizeof(T));
  }

  ~Matrix() { dealloc(); }

  std::string to_string() const {
    std::ostringstream ss;
    ss << *this;
    return ss.str();
  }

  unsigned rows() const { return _rows; }
  unsigned cols() const { return _cols; }
  unsigned size() const { return _cols * _rows; }

  T &at(unsigned row, unsigned col) {
    bound_check(row, col);
    return _array[index(row, col)];
  }

  T &at(unsigned row, unsigned col) const {
    if (row >= _rows || col >= _cols)
      throw std::out_of_range("out of bounds");
    return _array[index(row, col)];
  }

  Matrix &operator=(const Matrix &m) {
    //@todo
  }

  Matrix &operator+=(const Matrix &m) {
    //@todo
  }

  Matrix &operator-=(const Matrix &m) {
    //@todo
  }

  Matrix &operator*=(const Matrix &m) {
    //@todo
  }

  Matrix &operator*=(const T &value) {
    //@todo
  }

  Matrix &operator/=(const T &value) {
    //@todo
  }

  bool operator==(const Matrix &m) const {
    if (m.rows() != _rows)
      return false;
    else if (m.cols() != _cols)
      return false;

    for (auto i = _rows; i-- > 0;)
      for (auto j = _cols; j-- > 0;)
        if (at(i, j) != m.at(i, j))
          return false;

    return true;
  }

  bool operator!=(const Matrix &m) const { return !(*this == m); }
  const Matrix &operator+(const Matrix &m) { return (*this += m); }
  const Matrix &operator-(const Matrix &m) { return (*this -= m); }
  const Matrix &operator*(const Matrix &m) { return (*this *= m); }
  const Matrix &operator*(const T &value) { return (*this *= value); }

  void fill(const T &value) {
    // hopping for a compiler optimization :D
    for (unsigned i = 0; i < size(); ++i)
      _array[i] = value;
  }

  void print() {
    std::cout << "Rows: " << _rows << std::endl;
    std::cout << "Cols: " << _cols << std::endl;

    for (unsigned i = 0; i < _rows; i++) {
      std::cout << std::setw(3);
      for (unsigned j = 0; j < _cols; j++)
        std::cout << _array[index(i, j)] << std::setw(3);

      std::cout << std::endl;
    }
  }

  Matrix transpose() { return M_OPERATION<T>::transpose(*this); }

  void remove_row(unsigned row) {
    bound_check(row, _cols - 1);

    unsigned index_del = row * _cols;
    unsigned index_next = (row + 1) * _cols;

    T *tmp_array = new T[(_rows - 1) * _cols];

    std::memmove(tmp_array, _array, (index_del) * sizeof(T));
    std::memmove(tmp_array + index_del, _array + index_next, (size() - index_next) * sizeof(T));
    _rows -= 1;

    dealloc();
    _array = tmp_array;
  }

  void remove_col(unsigned col) {
    bound_check(_rows - 1, col);

    unsigned sIndexN = 0; // start index new array
    unsigned sIndexO = 0; // start index old array
    unsigned nIndex = 0;  // next index old array
    T *tmp_array = new T[_rows * (_cols - 1)];

    std::memmove(tmp_array, _array, col * sizeof(T));
    for (unsigned i = 0; i < _rows; ++i) {
      sIndexN = i * (_cols - 1) + col;
      sIndexO = i * _cols + col;
      nIndex = (i + 1) * _cols + col;
      std::memmove(tmp_array + sIndexN, _array + sIndexO + 1, (nIndex - sIndexO - 1) * sizeof(T));
    }

    _cols -= 1;
    dealloc();
    _array = tmp_array;
  }

  void reshape(unsigned rows, unsigned cols) {
    //@todo
  }

  void append_row(const std::initializer_list<T> &values) { insert_row(_rows, values); }

  void append_col(const std::initializer_list<T> &values) { insert_col(_cols, values); }

  void insert_row(unsigned index, const std::initializer_list<T> &values) {
    // @todo
  }

  void insert_col(unsigned index, const std::initializer_list<T> &values) {
    // @todo
  }

private:
  T *_array;
  unsigned _rows;
  unsigned _cols;

  unsigned index(unsigned row, unsigned col) const { return row * _cols + col; }

  void alloc() { _array = new T[_rows * _cols]; }

  void dealloc() {
    delete[] _array;
    _array = nullptr;
  }

  void bound_check(unsigned row, unsigned col) {
    if (row >= _rows || col >= _cols)
      throw std::out_of_range("out of range");
  }
};
/***************************************************************************************************
 *
 ***************************************************************************************************/
template <class T> inline std::ostream &operator<<(std::ostream &outs, const Matrix<T> &m) {
  return outs << "Matrix<" << typeid(T).name() << ">[" << m.rows() << "][" << m.cols() << "]";
}

#endif // MATRIX_H
/***************************************************************************************************
 *
 ***************************************************************************************************/
