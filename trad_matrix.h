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

template <typename T> class trad_matrix {
private:
  std::vector<T> _array;
  sz_t size_x;
  sz_t size_y;

public:
  trad_matrix() : size_x(0), size_y(0) {}
  trad_matrix(const std::size_t &n, const std::size_t &m)
      : _array(n * m), size_x(n), size_y(m) {}
  trad_matrix(const std::size_t &n, const std::size_t &m, const T &val)
      : _array(n * m, val), size_x(n), size_y(m) {}
  trad_matrix(const std::vector<std::vector<T>> &vec)
      : size_x(vec.size()), size_y((*vec.begin()).size()) {
    for (const auto it : vec) {
      _array.insert(_array.begin(), it.begin(), it.end());
    }
  }
  trad_matrix(const List<T> &l)
      : size_x(l.size()), size_y((*l.begin()).size()) {
    for (const auto it : l) {
      _array.insert(_array.begin(), it.begin(), it.end());
    }
  }
  std::pair<sz_t, sz_t> shape() const { return std::make_pair(size_x, size_y); }

  inline const T operator()(const std::size_t i, const std::size_t j) const {
    return _array[i * size_y + j];
  }
  inline T &operator()(const std::size_t i, const std::size_t j) {
    return _array[i * size_y + j];
  }
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

  template <typename R2> decltype(auto) operator+(const R2 &b) {
    assert(shape() == b.shape());
    trad_matrix<decltype((*this)(0, 0) + b(0, 0))> temp(size_x, size_y);
    for (int i = 0; i < size_x; i++) {
      for (int j = 0; j < size_y; j++) {
        temp(i, j) = (*this)(i, j) + b(i, j);
      }
    }
    return temp;
  }
  template <typename R1> void operator+=(const R1 &other) {
    *this = *this + other;
  }
  template <typename R2> decltype(auto) operator-(const R2 &b) {
    assert(shape() == b.shape());
    trad_matrix<decltype((*this)(0, 0) + b(0, 0))> temp(size_x, size_y);
    for (int i = 0; i < size_x; i++) {
      for (int j = 0; j < size_y; j++) {
        temp(i, j) = (*this)(i, j) - b(i, j);
      }
    }
    return temp;
  }
  template <typename R1> void operator-=(const R1 &other) {
    *this = *this - other;
  }
  template <typename R2> decltype(auto) operator%(const R2 &b) {
    assert(shape().second == b.shape().first);
    sz_t p = size_x, q = size_y, r = b.shape().second;
    trad_matrix<decltype((*this)(0, 0) + b(0, 0))> temp(size_x, size_y);
    for (sz_t i = 0; i < p; i++) {
      for (sz_t j = 0; j < q; j++) {
        for (sz_t k = 0; k < r; k++) {
          temp(i, j) += (*this)(i, k) * b(k, j);
        }
      }
    }
    return temp;
  }
  template <typename R1> void operator%=(const R1 &other) {
    *this = (*this) % other;
  }
  template <typename R2> decltype(auto) operator*(const R2 &b) {
    assert(shape() == b.shape());
    trad_matrix<decltype((*this)(0, 0) * b(0, 0))> temp(size_x, size_y);
    for (int i = 0; i < size_x; i++) {
      for (int j = 0; j < size_y; j++) {
        temp(i, j) = (*this)(i, j) * b(i, j);
      }
    }
    return temp;
  }
  template <typename R1> void operator*=(const R1 &other) {
    *this = (*this) * other;
  }
  template <typename R2> decltype(auto) operator/(const R2 &b) {
    assert(shape() == b.shape());
    trad_matrix<decltype((*this)(0, 0) / b(0, 0))> temp(size_x, size_y);
    for (int i = 0; i < size_x; i++) {
      for (int j = 0; j < size_y; j++) {
        temp(i, j) = (*this)(i, j) / b(i, j);
      }
    }
    return temp;
  }
  template <typename R1> void operator/=(const R1 &other) {
    *this = *this / other;
  }
};
