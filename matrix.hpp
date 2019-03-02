#ifndef __MATRIX_H
#define __MATRIX_H
#include <cassert>
#include <iostream>
#include <vector>
/***************************************************************************/
/*********************DEFAULT CONTAINER CLASS  FOR MATRIX******************/
template <typename T>
class default_container {
 public:
  std::vector<std::vector<T>> __array;
  std::size_t __size_x;
  std::size_t __size_y;
  // -----------------------Constructors------------------------------------//
  default_container() : __size_x(0), __size_y(0) {}
  default_container(const std::size_t sx, const std::size_t sy)
      : __size_x(sx), __size_y(sy), __array(sx, std::vector<T>(sy)) {}
  default_container(const std::size_t sx, const std::size_t sy, const T val)
      : __size_x(sx), __size_y(sy), __array(sx, std::vector<T>(sy, val)) {}
  default_container(const std::vector<std::vector<T>>&);
  default_container(const std::initializer_list<std::initializer_list<T>>&);
  //------------------------------------------------------------------------//
  //--------------------------Destructor------------------------------------//
  ~default_container() {}
  //------------------------------------------------------------------------//
  //-----------------------overloading operator ()--------------------------//
  const T operator()(const std::size_t i, const std::size_t j) const {
    return __array[i][j];
  }

  T& operator()(const std::size_t i, const std::size_t j) {
    return __array[i][j];
  }
  //-----------------------------------------------------------------------//
};
//-------------------------Constructors body ------------------------------//
template <typename T>
default_container<T>::default_container(
    const std::vector<std::vector<T>>& other)
    : __size_x(other.size()),
      __size_y((*other.begin()).size()),
      __array(other) {}
template <typename T>
default_container<T>::default_container(
    const std::initializer_list<std::initializer_list<T>>& other)
    : __size_x(other.size()), __size_y((*other.begin()).size()) {
  for (auto& it : other) {
    __array.push_back(it);
  }
}
// ------------------------------------------------------------------------//
/*******************DEFAULT CONTAINER CLASS  FOR MATRIX Ends****************/
/***************************************************************************/

/***************************************************************************/
/*******************************MATRIX CLASS********************************/
template <typename T, typename Type = default_container<T>>
class matrix {
 private:
  Type __MATRIX;

 public:
  // -----------------------Constructors------------------------------------//
  matrix(){};
  matrix(const std::size_t sx, const std::size_t sy) : __MATRIX(sx, sy) {}
  matrix(const std::size_t sx, const std::size_t sy, const T val)
      : __MATRIX(sx, sy, val) {}
  matrix(const std::vector<std::vector<T>>& other) : __MATRIX(other) {}
  matrix(const std::initializer_list<std::initializer_list<T>>& other)
      : __MATRIX(other) {}
  matrix(const Type& other) : __MATRIX(other) {}
  //------------------------------------------------------------------------//
  //--------------------------Destructor------------------------------------//
  ~matrix() {}
  //------------------------------------------------------------------------//

  //-----------------------overloading operator ()--------------------------//
  T operator()(const std::size_t i, const std::size_t j) const {
    return __MATRIX(i, j);
  }
  T& operator()(const std::size_t i, const std::size_t j) {
    return __MATRIX(i, j);
  }
  //------------------------------------------------------------------------//

  //-------------------------assignment prototype---------------------------//
  template <typename T1, typename Type1>
  matrix<T, Type>& operator=(const matrix<T1, Type1>&);
  
  //------------------------------------------------------------------------//

  //-----------------------Getting The Data And Dimention of MATRIX---------//
  // Getting data of matrix
  const Type& data() const { return __MATRIX; }
  Type& data() { return __MATRIX; }
  // gitting dimentions of __MATRIX
  std::pair<std::size_t, std::size_t> shape() const {
    return std::make_pair(__MATRIX.__size_x, __MATRIX.__size_y);
  }
  std::size_t sizex() const { return __MATRIX.__size_x; }
  std::size_t sizey() const { return __MATRIX.__size_y; }
  //------------------------------------------------------------------------//
};
//--------------------------Matrix Assignment Body--------------------------//
template <typename T, typename Type>
template <typename T1, typename Type1>
matrix<T, Type>& matrix<T,Type>::operator=(const matrix<T1, Type1>& other) {
  assert(shape() == other.shape());
  std::size_t tx = sizex();
  std::size_t ty = sizey();
  for (std::size_t i = 0; i < tx; i++) {
    for (std::size_t j = 0; j < ty; j++) {
      __MATRIX(i, j) = other(i, j);
    }
  }
  return *this;
}
//--------------------------------------------------------------------------//
//-----------------------overloading operator << --------------------------//
  template <typename T, typename F>
std::ostream& operator<<(std::ostream& out, const matrix<T, F>& a) {
  std::size_t tx = a.sizex();
  std::size_t ty = a.sizey();
  for (size_t i = 0; i < tx; i++) {
    for (size_t j = 0; j < ty; j++) {
      out << a(i, j) << ' ';
    }
    out << std::endl;
  }
  return out;
}
//------------------------------------------------------------------------//
/*********************************MATRIX CLASS ENDS**************************/
/***************************************************************************/

/***************************************************************************/
/**********************MATRIX ADDITION CLASS********************************/
template <typename T, typename R1, typename R2>
class ADD {
 public:
  const R1& operand1;
  const R2& operand2;
  std::size_t __size_x;
  std::size_t __size_y;
  ADD(const R1& a, const R2& b)
      : operand1(a),
        operand2(b),
        __size_x(operand1.__size_x),
        __size_y(operand1.__size_y) {}
  T operator()(const std::size_t i, const std::size_t j) const {
    return operand1(i, j) + operand2(i, j);
  }
};
//---------------Overloading Operator + for adding two MATRIX----------------/
template <typename T, typename R1, typename R2>
matrix<T, ADD<T, R1, R2>> operator+(matrix<T, R1> const& a,
                                    matrix<T, R2> const& b) {
  return matrix<T, ADD<T, R1, R2>>(ADD<T, R1, R2>(a.data(), b.data()));
}
/************************MATRIX ADDITION CLASS ENDS**************************/
/***************************************************************************/

#endif
