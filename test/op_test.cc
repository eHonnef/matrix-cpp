#define CATCH_CONFIG_MAIN
#include "../Operations.cc"
#include "catch/catch.hpp"

TEST_CASE("Matrix transposition", "[Matrix transposition]") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  Matrix<int> b = M_OPERATION::transpose(a);
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

  a = M_OPERATION::u_triangular(a);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = i; j-- > i;)
      REQUIRE(a.at(i, j) == 7);

  for (unsigned i = 1; i < a.rows(); ++i)
    for (unsigned j = 0; j < i; ++j)
      REQUIRE(a.at(i, j) == 0);
}

TEST_CASE("Lower triangular matrix", "[Lower triangular matrix]") {
  Matrix<int> a(10, 10, 7);

  a = M_OPERATION::l_triangular(a);
  for (auto i = a.rows(); i-- > 0;)
    for (auto j = i; j-- > i;)
      REQUIRE(a.at(i, j) == 0);

  for (unsigned i = 1; i < a.rows(); ++i)
    for (unsigned j = 0; j < i; ++j)
      REQUIRE(a.at(i, j) == 7);
}

TEST_CASE("Matrix determinant", "[Matrix determinant]") {
  Matrix<int> a({{1, 1, 2}, {3, 4, 5}, {6, 7, 8}});
  REQUIRE(M_OPERATION::determinant(a) == -3);

  // Matrix<int> b(20, 20);
  // for (auto i = b.size(); i-- > 0;)
  //   b.array_access(i) = i + 1;

  // REQUIRE(b.determinant() == -5e-245);
}

TEST_CASE("Matrix inverse", "[Matrix inverse]") {}
