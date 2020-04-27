#define CATCH_CONFIG_MAIN
#include "../Matrix.cc"
#include "catch/catch.hpp"

TEST_CASE("Matrix to string/matrix name", "[Matrix to string/Matrix name]") {
  Matrix<int> a(5, 8);
  std::cout << "Print test:" << std::endl;
  std::cout << "This should print \"Matrix<i>[5][8]\": " << a << std::endl;
  std::cout << "This should also print \"Matrix<i>[5][8]\": " << a.to_string() << std::endl;
  a.print();
}

TEST_CASE("Matrix errors", "[Matrix errors]") {
  REQUIRE_THROWS(Matrix<int>({{0, 1, 2}, {3, 4, 5}, {6, 7}}));
  REQUIRE_THROWS(Matrix<int>({{0, 1, 2}, {3, 4, 5, 7}, {6, 7, 8}}));

  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  REQUIRE_THROWS(a.at(10, 10));
}

TEST_CASE("Matrix structure", "[Matrix structure]") {
  Matrix<int> a(5, 8);
  REQUIRE(a.rows() == 5);
  REQUIRE(a.cols() == 8);
  REQUIRE(a.size() == (5 * 8));
}

TEST_CASE("Matrix access and values", "[Matrix access and values]") {
  Matrix<int> a(5, 8);
  Matrix<int> b(5, 8, 10);
  a[4][4] = 123;
  a.at(3, 3) = 987;

  b[4][4] = 1234;
  b.at(3, 3) = 654;

  REQUIRE(a[1][3] == int());
  REQUIRE(a[4][4] == 123);
  REQUIRE(a.at(1, 3) == int());
  REQUIRE(a.at(4, 4) == 123);
  REQUIRE(a.at(3, 3) == 987);
  REQUIRE(b[4][4] == 1234);
  REQUIRE(b[1][4] == 10);
  REQUIRE(b.at(4, 4) == 1234);
  REQUIRE(b.at(3, 3) == 654);
  REQUIRE(b.at(1, 4) == 10);
}

TEST_CASE("Matrix brace initialize", "[Matrix brace initialize]") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  REQUIRE(a.rows() == 2);
  REQUIRE(a.cols() == 3);
  REQUIRE(a[0][0] == 0);
  REQUIRE(a[0][1] == 1);
  REQUIRE(a[0][2] == 2);
  REQUIRE(a[1][0] == 3);
  REQUIRE(a[1][1] == 4);
  REQUIRE(a[1][2] == 5);
}

TEST_CASE("Matrix row deallocation", "[Matrix row deallocation]") {
  Matrix<int> a(5, 8, 5);
  REQUIRE_THROWS(a.remove_row(5));

  // setting the values of row 1 to 3
  for (auto i = a.cols(); i-- > 0;)
    a[1][i] = 3;
  a.remove_row(1);

  REQUIRE(a.rows() == 4);
  REQUIRE(a[1][3] != 3);
}

TEST_CASE("Matrix col deallocation", "[Matrix col deallocation]") {
  Matrix<int> a(5, 8, 5);
  REQUIRE_THROWS(a.remove_col(9));

  // setting the values of col 3 to 9
  for (auto i = a.rows(); i-- > 0;)
    a[i][3] = 9;
  a.remove_col(3);

  REQUIRE(a.cols() == 7);
  REQUIRE(a[3][3] != 9);
}

TEST_CASE("Matrix row insertion", "[Matrix row insertion]") {}
TEST_CASE("Matrix append row", "[Matrix append row]") {}

TEST_CASE("Matrix col insertion", "[Matrix col insertion]") {}
TEST_CASE("Matrix append col", "[Matrix append col]") {}

TEST_CASE("Matrix equality", "[Matrix equality]") {
  Matrix<int> a(5, 8);
  Matrix<int> b(5, 8, 10);
  Matrix<int> c(5, 8, 10);
  Matrix<int> d(5, 7, 10);

  REQUIRE(a != b);
  REQUIRE(b == c);
  REQUIRE(c != d);
}

TEST_CASE("Matrix transposition", "[Matrix transposition]") {
  Matrix<int> a({{0, 1, 2}, {3, 4, 5}});
  Matrix<int> b = a.transpose();
  // {0,3}
  // {1,4}
  // {2,5}
  REQUIRE(b.rows() == 3);
  REQUIRE(b.cols() == 2);
  REQUIRE(b[0][0] == 0);
  REQUIRE(b[0][1] == 3);
  REQUIRE(b[1][0] == 1);
  REQUIRE(b[1][1] == 4);
  REQUIRE(b[2][0] == 2);
  REQUIRE(b[2][1] == 5);
}

TEST_CASE("Matrices multiplication", "[Matrices multiplication]") {}
TEST_CASE("Matrix scalar multiplication", "[Matrix scalar multiplication]") {}
TEST_CASE("Sum of Matrices", "[Sum of Matrices]") {}
TEST_CASE("Matrices subtraction", "[Matrices subtraction]") {}
TEST_CASE("Matrix inverse", "[Matrix inverse]") {}