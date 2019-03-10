#pragma once
#include <cassert>
#include <iostream>
#include <type_traits>
#include <vector>
using namespace std;
using sz_t = std::size_t;
template <typename T> using TDVec = std::vector<std::vector<T>>;
template <typename T>
using List = std::initializer_list<std::initializer_list<T>>;
/**
 * @brief      Class for trad matrix (traditional matrix)
 *
 * @tparam     T     Data type of the matrix
 */
template <typename T> class trad_matrix {
private:
  std::vector<T> _array;
  sz_t size_x;
  sz_t size_y;

public:
  /**
   * @brief      Constructs the object.
   */
  trad_matrix() : size_x(0), size_y(0) {}
  /**
   * @brief      Constructs the object.
   *
   * @param[in]  n     Number of rows in the matrix
   * @param[in]  m     Number of columns in the matrix
   */
  trad_matrix(const std::size_t &n, const std::size_t &m)
      : _array(n * m), size_x(n), size_y(m) {}
  /**
   * @brief      Constructs the object.
   *
   * @param[in]  n     Number of rows in the matrix
   * @param[in]  m     Number of columns in the matrix
   * @param[in]  val   The initial value
   */
  trad_matrix(const std::size_t &n, const std::size_t &m, const T &val)
      : _array(n * m, val), size_x(n), size_y(m) {}
  /**
   * @brief      Vector initialization
   *
   * @param[in]  vec   2D Vector input
   */
  trad_matrix(const TDVec<T> &vec)
      : size_x(vec.size()), size_y((*vec.begin()).size()) {
    for (const auto it : vec) {
      _array.insert(_array.begin(), it.begin(), it.end());
    }
  }
  /**
   * @brief      Initialization with 2D list
   *
   * @param[in]  l     2D Initializer_list input
   */
  trad_matrix(const List<T> &l)
      : size_x(l.size()), size_y((*l.begin()).size()) {
    for (const auto it : l) {
      _array.insert(_array.begin(), it.begin(), it.end());
    }
  }
  /**
   * @brief      Gives the dimensions of the matrix
   */
  std::pair<sz_t, sz_t> shape() const { return std::make_pair(size_x, size_y); }
  /**
   * Operator () Overloading for getting the (i,j)th element
   */
  inline const T operator()(const std::size_t i, const std::size_t j) const {
    return _array[i * size_y + j];
  }
  inline T &operator()(const std::size_t i, const std::size_t j) {
    return _array[i * size_y + j];
  }
  /**
   * @brief      Oveloading operator << to use std:: cout
   */
  friend std::ostream &operator<<(std::ostream &out, trad_matrix<T> &a) {
    sz_t size_x = a.shape().first;
    sz_t size_y = a.shape().second;
    for (sz_t i = 0; i < size_x; i++) {
      for (sz_t j = 0; j < size_y; j++) {
        out << a(i, j) << ' ';
      }
      out << std::endl;
    }
    return out;
  }
  /**
   * @brief      Overloading operator = for a assignment of lazy matrix
   *
   * @param[in]  other  reference to the matrix which is to be
   *                    assigned
   *
   * @tparam     R1     matrix type
   */
  template <typename R1> trad_matrix<T> operator=(const R1 &other) {
    assert(shape() == other.shape());
    trad_matrix<T> temp(size_x, size_y);
    for (sz_t i = 0; i < size_x; i++) {
      for (sz_t j = 0; j < size_y; j++) {
        temp(i, j) = other(i, j);
      }
    }
    for (sz_t i = 0; i < size_x; i++) {
      for (sz_t j = 0; j < size_y; j++) {
        (*this)(i, j) = other(i, j);
      }
    }
    return *this;
  }
  /**
   * @brief      Overloading operator == for a comparing equality with other
   *             matrix
   *
   * @param[in]  other  reference to the matrix  which is to be
   *                    compared
   *
   * @tparam     R2     matrix
   *
   * @return     true if eaual else false
   */
  template <typename R1> bool operator==(const R1 &other) {
    if (shape() != other.shape() && typeid(T) != typeid(other(0, 0))) {
      return false;
    }
    for (sz_t i = 0; i < size_x; i++) {
      for (sz_t j = 0; j < size_y; j++) {
        if ((*this)(i, j) != other(i, j)) {
          return false;
        }
      }
    }
    return true;
  }
  /**
   * @brief      Operator + Overloading for Standard Matrix Addition
   *
   * @param[in]  other  reference to the matrix which is to be
   *                    added
   *
   * @tparam     R1     matrix
   */
  template <typename R1> decltype(auto) operator+(const R1 &other) {
    assert(shape() == other.shape());
    trad_matrix<decltype((*this)(0, 0) + b(0, 0))> temp(size_x, size_y);
    for (int i = 0; i < size_x; i++) {
      for (int j = 0; j < size_y; j++) {
        temp(i, j) = (*this)(i, j) + other(i, j);
      }
    }
    return temp;
  }
  /**
   * @brief      assignment after adding
   */
  template <typename R1> void operator+=(const R1 &other) {
    *this = *this + other;
  }
  /**
   * @brief      Operator - Overloading for Standard Matrix Subtraction
   *
   * @param[in]  other  reference to the matrix which is to be
   *                    subtracted
   *
   * @tparam     R1     matrix
   */
  template <typename R1> decltype(auto) operator-(const R1 &b) {
    assert(shape() == other.shape());
    trad_matrix<decltype((*this)(0, 0) + b(0, 0))> temp(size_x, size_y);
    for (int i = 0; i < size_x; i++) {
      for (int j = 0; j < size_y; j++) {
        temp(i, j) = (*this)(i, j) - other(i, j);
      }
    }
    return temp;
  }
  /**
   * @brief      assignment after subtracting
   */
  template <typename R1> void operator-=(const R1 &other) {
    *this = *this - other;
  }
  /**
   * @brief      Operator / Overloading for Element-Wise Division
   *
   * @param[in]  other  reference to the matrix or expression which is to be
   *                    divided
   *
   * @tparam     R1     matrix
   */
  template <typename R1> decltype(auto) operator/(const R1 &other) {
    assert(shape() == other.shape());
    trad_matrix<decltype((*this)(0, 0) / other(0, 0))> temp(size_x, size_y);
    for (int i = 0; i < size_x; i++) {
      for (int j = 0; j < size_y; j++) {
        temp(i, j) = (*this)(i, j) / other(i, j);
      }
    }
    return temp;
  }
  /**
   * @brief      assignment after element-wise division
   */
  template <typename R1> void operator/=(const R1 &other) {
    *this = *this / other;
  }
  /**
   * @brief      Operator * Overloading for Element-Wise Multiplication
   *
   * @param[in]  other  reference to the matrix or expression which is to be
   *                    multiplied
   *
   * @tparam     R1     matrix
   */
  template <typename R1> decltype(auto) operator*(const R1 &other) {
    assert(shape() == other.shape());
    trad_matrix<decltype((*this)(0, 0) * other(0, 0))> temp(size_x, size_y);
    for (int i = 0; i < size_x; i++) {
      for (int j = 0; j < size_y; j++) {
        temp(i, j) = (*this)(i, j) * other(i, j);
      }
    }
    return temp;
  }
  /**
   * @brief      assignment after element-wise multiplication
   */
  template <typename R1> void operator*=(const R1 &other) {
    *this = (*this) * other;
  }
  /**
   * @brief      Operator % Overloading for Standard Matrix Multiplication
   *
   * @param[in]  other  reference to the matrix or expression which is to be
   *                    multiplied
   *
   * @tparam     R1     matrix
   */
  template <typename R1> decltype(auto) operator%(const R1 &other) {
    assert(shape().second == b.shape().first);
    sz_t p = size_x, q = size_y, r = other.shape().second;
    trad_matrix<decltype((*this)(0, 0) + b(0, 0))> temp(size_x, size_y);
    for (sz_t i = 0; i < p; i++) {
      for (sz_t j = 0; j < q; j++) {
        for (sz_t k = 0; k < r; k++) {
          temp(i, j) += (*this)(i, k) * other(k, j);
        }
      }
    }
    return temp;
  }
  /**
   * @brief      assignment after standard matrix multiplication
   */
  template <typename R1> void operator%=(const R1 &other) {
    *this = (*this) % other;
  }
};
