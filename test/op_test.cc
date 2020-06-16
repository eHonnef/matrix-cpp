#define CATCH_CONFIG_MAIN
#include "../basic_op.cc"
#include "catch/catch.hpp"

typedef basic_op OP;

TEST_CASE("Matrix transposition", "[Matrix transposition]") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  Matrix<int> b = OP::transpose(a);
  // {0,3}
  // {1,4}
  // {2,5}
  REQUIRE(b.rows() == 3);
  REQUIRE(b.cols() == 2);
  REQUIRE(b.at(0, 0) == 0);
  REQUIRE(b.at(0, 1) == 3);
  REQUIRE(b.at(1, 0) == 1);
  REQUIRE(b.at(1, 1) == 4);
  REQUIRE(b.at(2, 0) == 2);
  REQUIRE(b.at(2, 1) == 5);
}

TEST_CASE("Upper triangular matrix", "[Upper triangular matrix]") {
  Matrix<int> a(10, 10, 7);

  a = OP::u_triangular(a);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = i; j-- > i;)
      REQUIRE(a.at(i, j) == 7);

  for (unsigned i = 1; i < a.rows(); ++i)
    for (unsigned j = 0; j < i; ++j)
      REQUIRE(a.at(i, j) == 0);
}

TEST_CASE("Lower triangular matrix", "[Lower triangular matrix]") {
  Matrix<int> a(10, 10, 7);

  a = OP::l_triangular(a);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = i; j-- > i;)
      REQUIRE(a.at(i, j) == 0);

  for (unsigned i = 1; i < a.rows(); ++i)
    for (unsigned j = 0; j < i; ++j)
      REQUIRE(a.at(i, j) == 7);
}

TEST_CASE("LU decomposition", "[LU decomposition]") {
  Matrix<double> a({{0.448, 0.832, 0.193}, {0.421, 0.784, -0.207}, {-0.319, 0.884, 0.279}});
  Matrix<double> LU(a.rows(), a.cols());

  OP::LUP_crout(a, LU);

  REQUIRE(LU.at(0, 0) == Approx(0.448).epsilon(0.001));
  REQUIRE(LU.at(0, 1) == Approx(1.857).epsilon(0.001));
  REQUIRE(LU.at(0, 2) == Approx(0.4308).epsilon(0.0001));
  REQUIRE(LU.at(1, 0) == Approx(-0.319).epsilon(0.001));
  REQUIRE(LU.at(1, 1) == Approx(1.476).epsilon(0.001));
  REQUIRE(LU.at(1, 2) == Approx(0.2821).epsilon(0.001));
  REQUIRE(LU.at(2, 0) == Approx(0.421).epsilon(0.001));
  REQUIRE(LU.at(2, 1) == Approx(0.00214).epsilon(0.01));
  REQUIRE(LU.at(2, 2) == Approx(-0.3890).epsilon(0.0001));
}

TEST_CASE("LU decomposition split", "[LU decomposition split]") {
  Matrix<int> LU({{1, 2, 3}, {7, 8, 4}, {5, 6, 9}});
  Matrix<int> L(LU.rows(), LU.rows());
  Matrix<int> U(LU.rows(), LU.rows());

  OP::split_LU_crout(LU, L, U);

  REQUIRE(L.at(0, 0) == 1);
  REQUIRE(L.at(0, 1) == 0);
  REQUIRE(L.at(0, 2) == 0);
  REQUIRE(L.at(1, 0) == 7);
  REQUIRE(L.at(1, 1) == 8);
  REQUIRE(L.at(1, 2) == 0);
  REQUIRE(L.at(2, 0) == 5);
  REQUIRE(L.at(2, 1) == 6);
  REQUIRE(L.at(2, 2) == 9);

  REQUIRE(U.at(0, 0) == 1);
  REQUIRE(U.at(0, 1) == 2);
  REQUIRE(U.at(0, 2) == 3);
  REQUIRE(U.at(1, 0) == 0);
  REQUIRE(U.at(1, 1) == 1);
  REQUIRE(U.at(1, 2) == 4);
  REQUIRE(U.at(2, 0) == 0);
  REQUIRE(U.at(2, 1) == 0);
  REQUIRE(U.at(2, 2) == 1);
}

TEST_CASE("Matrix determinant", "[Matrix determinant]") {
  Matrix<double> a({{0.448, 0.832, 0.193}, {0.421, 0.784, -0.207}, {-0.319, 0.884, 0.279}});
  REQUIRE(OP::determinant(a) == Approx(0.2572821));
}

TEST_CASE("Identity Matrix", "[Identity Matrix]") {
  Matrix<int> a = OP::identity<int>(10);

  for (auto i = 0; i < 10; ++i) {
    for (auto j = 0; j < 10; ++j) {
      if (i == j)
        REQUIRE(a.at(i, j) == 1);
      else
        REQUIRE(a.at(i, j) == 0);
    }
  }
}

TEST_CASE("Inverse Matrix", "[Inverse Matrix]") {
  Matrix<double> a({{1, 2, 3}, {0, 1, 4}, {5, 6, 0}});
  a = OP::inverse(a);

  REQUIRE(a.at(0, 0) == Approx(-24));
  REQUIRE(a.at(0, 1) == Approx(18));
  REQUIRE(a.at(0, 2) == Approx(5));
  REQUIRE(a.at(1, 0) == Approx(20));
  REQUIRE(a.at(1, 1) == Approx(-15));
  REQUIRE(a.at(1, 2) == Approx(-4));
  REQUIRE(a.at(2, 0) == Approx(-5));
  REQUIRE(a.at(2, 1) == Approx(4));
  REQUIRE(a.at(2, 2) == Approx(1));
}

TEST_CASE("Further LU decomposition test", "[Further LU decomposition test]") {
  // PA = LU
  // det(A) == det(LU) == det(L'U')
  // Matrix<double> A({{1, 2, 3, 4}, {1, 2, -3, -4}, {-1, 0, 2, 3}, {1, -4, -1, 1}});
  // Matrix<double> LU;
  // Matrix<int> P;

  // P = OP::LUP_crout(A, LU);
  // Matrix<double> b = OP::multiplication(L, U);
  // Matrix<double> c = OP::multiplication(P, A);
  // Matrix<double> d = OP::multiplication(OP::inverse(L), OP::inverse(U));

  // // REQUIRE(c == b); @todo: change the matrix operator==
  // REQUIRE(OP::determinant(A) == Approx(OP::determinant(b)));
  // REQUIRE(OP::determinant(d) == Approx(OP::determinant(A)));
  // REQUIRE(OP::determinant(d) == Approx(OP::determinant(c)));
}
