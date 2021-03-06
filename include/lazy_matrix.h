#pragma once
#include <cassert>
#include <iostream>
#include <typeinfo>
#include <vector>

using sz_t = std::size_t;
template <typename T> using TDVec = std::vector<std::vector<T>>;
template <typename T>
using List = std::initializer_list<std::initializer_list<T>>;

namespace policy {
/**
 * @brief      policy that take row as first priority
 */
class row_major {
public:
  template <typename R1>
  decltype(auto) operator()(const sz_t &i, const sz_t &j, const R1 &a) const {
    return a._array[i * a.size_y + j];
  }
  template <typename R1>
  decltype(auto) operator()(const sz_t &i, const sz_t &j, R1 &a) {
    return a._array[i * a.size_y + j];
  }
};

/**
 * @brief      policy that take column as first priority
 */
class column_major {
public:
  template <typename R1>
  decltype(auto) operator()(const sz_t &i, const sz_t &j, const R1 &a) const {
    return a._array[j * a.size_x + i];
  }
  template <typename R1>
  decltype(auto) operator()(const sz_t &i, const sz_t &j, R1 &a) {
    return a._array[j * a.size_x + i];
  }
};
}; // namespace policy

/**
 * @brief      Functor for adding corresponding position
 */
struct _add {
  template <typename R1, typename R2>
  decltype(auto) operator()(const R1 &op1, const R2 &op2, const sz_t &i,
                            const sz_t &j) const {
    return op1(i, j) + op2(i, j);
  }
};

/**
 * @brief      Functor for subtracting corresponding position
 */
struct _sub {
  template <typename R1, typename R2>
  decltype(auto) operator()(const R1 &op1, const R2 &op2, const sz_t &i,
                            const sz_t &j) const {
    return op1(i, j) - op2(i, j);
  }
};

/**
 * @brief      Functor for dividing corresponding position
 */
struct _ediv {
  template <typename R1, typename R2>
  decltype(auto) operator()(const R1 &op1, const R2 &op2, const sz_t &i,
                            const sz_t &j) const {
    return op1(i, j) / op2(i, j);
  }
};

/**
 * @brief      Functor for diving with a scalar
 */
struct _sdiv {
  template <typename R1, typename R2>
  decltype(auto) operator()(const R1 &op1, const R2 &op2, const sz_t &i,
                            const sz_t &j) const {
    return op1(i, j) / op2;
  }
};

/**
 * @brief      Functor for multiplying corresponding position
 */
struct _emul {
  template <typename R1, typename R2>
  decltype(auto) operator()(const R1 &op1, const R2 &op2, const sz_t &i,
                            const sz_t &j) const {
    return op1(i, j) * op2(i, j);
  }
};

/**
 * @brief      Functor for multiplying with scalar
 */
struct _smul {
  template <typename R1, typename R2>
  decltype(auto) operator()(const R1 &op1, const R2 &op2, const sz_t &i,
                            const sz_t &j) const {
    return op1(i, j) * op2;
  }
};

/**
 * @brief      Functor for getting (i,j)th element of Standard Matrix-Matrix
 *             multiplication
 */
struct _std_mul {
  template <typename R1, typename R2>
  decltype(auto) operator()(const R1 &op1, const R2 &op2, const sz_t &i,
                            const sz_t &j) const {
    return (_std_mul::operator()(op1, op2, op2.shape().first - 1, i, j));
  }
  template <typename R1, typename R2>
  decltype(auto) operator()(const R1 &op1, const R2 &op2, const sz_t &k,
                            const sz_t &i, const sz_t &j) const {
    if (k == 0) {
#pragma omp parallel single
      {
#pragma omp task
        decltype(auto) temp1 = op1(i, k);
#pragma omp task
        decltype(auto) temp2 = op2(k, j);
#pragma omp taskwait
        return temp1 * temp2;
      }
    } else {
#pragma omp parallel single
      {
#pragma omp task
        decltype(auto) temp1 = op1(i, k);
#pragma omp task
        decltype(auto) temp2 = op2(k, j);
#pragma omp task
        decltype(auto) temp3 = _std_mul::operator()(op1, op2, k - 1, i, j);
#pragma omp taskwait
        return temp1 * temp2 + temp3;
      }
    }
  }
};

/**
 * @brief      Class for expression.
 *
 * @tparam     R1    First Parameter
 * @tparam     R2    Fecond Parameter
 * @tparam     Op    Functor for atithematic operations
 */
template <typename R1, typename R2, typename Op> class expr {
private:
  const R1 &op1;
  const R2 &op2;
  const sz_t size_x;
  const sz_t size_y;
  Op op;

public:
  /**
   * @brief      Constructs the object.
   *
   * @param[in]  a     First Operand
   * @param[in]  b     Second Operand
   * @param[in]  f     Functor for Operator
   */
  expr(const R1 &a, const R2 &b, Op f)
      : op1(a), op2(b), size_x(a.shape().first), size_y(b.shape().second),
        op(f) {}
  /**
   * @brief      Gives the dimensions of the resultant expression
   *
   * @return     return a pair whose first value is the number of rows and
   *             the second value is the number of columns
   */
  decltype(auto) shape() const { return std::make_pair(size_x, size_y); }

  /**
   * @brief      Oveloading operator << to use std:: cout
   */
  friend std::ostream &operator<<(std::ostream &out, expr<R1, R2, Op> &other) {
    sz_t size_x = other.shape().first;
    sz_t size_y = other.shape().second;
#pragma omp parallel for
    for (sz_t i = 0; i < size_x; i++) {
      for (sz_t j = 0; j < size_y; j++) {
        out << other(i, j) << ' ';
      }
      out << std::endl;
    }
    return out;
  }
  /**
   * Operator + Overloading for Standard Matrix Addition
   */
  template <typename F> decltype(auto) operator+(const F &other) {
    assert(shape() == other.shape());
    return expr<expr<R1, R2, Op>, F, _add>(*this, other, _add());
  }

  /**
   * Operator - Overloading for Standard Matrix Subtraction
   */
  template <typename F> decltype(auto) operator-(const F &other) {
    assert(shape() == other.shape());
    return expr<expr<R1, R2, Op>, F, _sub>(*this, other, _sub());
  }

  /**
   * Operator / Overloading for Element-Wise Division
   */

  template <typename F> decltype(auto) operator/(const F &other) {
    assert(shape() == other.shape());
    return expr<expr<R1, R2, Op>, F, _ediv>(*this, other, _ediv());
  }

  /**
   * Operator * Overloading for Element-Wise Multiplication
   */
  template <typename F> decltype(auto) operator*(const F &other) {
    assert(shape() == other.shape());
    return expr<expr<R1, R2, Op>, F, _emul>(*this, other, _emul());
  }

  /**
   * Operator % Overloading for Standard Matrix Multiplication
   */
  template <typename F> decltype(auto) operator%(const F &other) {
    assert(size_y == other.shape().second);
    return expr<expr<R1, R2, Op>, F, _std_mul>(*this, other, _std_mul());
  }
  /**
   * Operator () overloading for gitting the (i,j)th element of the
   * expression
   */
  decltype(auto) operator()(const sz_t &i, const sz_t &j) const {
    return op(op1, op2, i, j);
  }
};

/**
 * @brief      Class for lazy matrix.
 *
 * @tparam     T       Data type of the matrix
 * @tparam     policy  User case assign how data will be accessed takes
 *             value policy:: row_major or policy::column_major
 */
template <typename T, typename ploy = policy::row_major> class lazy_matrix {
private:
  std::vector<T> _array;
  const sz_t size_x;
  const sz_t size_y;
  friend ploy;
  ploy pol;

public:
  /**
   * @brief      Constructs the object.
   */
  lazy_matrix() : size_x(0), size_y(0) {}

  /**
   * @brief      Constructs the object.
   *
   * @param[in]  n     Number of rows in the matrix
   * @param[in]  m     Number of columns in the matrix
   */
  lazy_matrix(const std::size_t &n, const std::size_t &m)
      : _array(n * m), size_x(n), size_y(m) {}

  /**
   * @brief      Constructs the object.
   *
   * @param[in]  n     Number of rows in the matrix
   * @param[in]  m     Number of columns in the matrix
   * @param[in]  val   The initial value
   */
  lazy_matrix(const std::size_t &n, const std::size_t &m, const T &val)
      : _array(n * m, val), size_x(n), size_y(m) {}

  /**
   * @brief      Vector initialization
   *
   * @param[in]  vec   2D Vector input
   */
  lazy_matrix(const TDVec<T> &vec)
      : size_x(vec.size()), size_y((*vec.begin()).size()) {
    if (typeid(ploy) == typeid(policy::row_major)) {
      for (auto &it : vec) {
        _array.insert(_array.end(), it.begin(), it.end());
      }
    } else {
      for (sz_t j = 0; j < size_y; j++) {
        for (sz_t i = 0; i < size_x; i++) {
          _array.push_back(vec[i][j]);
        }
      }
    }
  }

  /**
   * @brief      Initialization with 2D list
   *
   * @param[in]  l     2D Initializer_list input
   */
  lazy_matrix(const List<T> &l)
      : size_x(l.size()), size_y((*l.begin()).size()) {
    if (typeid(ploy) == typeid(policy::row_major)) {
      for (auto &it : l) {
        _array.insert(_array.end(), it.begin(), it.end());
      }
    } else {
      for (sz_t j = 0; j < size_y; j++) {
        for (sz_t i = 0; i < size_x; i++) {
          _array.push_back(*((*(l.begin() + i)).begin() + j));
        }
      }
    }
  }

  /**
   * @brief      Initialization with an expression
   *
   * @param[in]  exp   The expression initializer
   *
   * @tparam     R1    Left operand of the expression
   * @tparam     R2    Right operand of the expression
   * @tparam     R3    Functor for performing arithematic operation
   */
  template <typename R1, typename R2, typename R3>
  lazy_matrix(const expr<R1, R2, R3> &exp)
      : size_x(exp.shape().first), size_y(exp.shape().second) {
    if (typeid(ploy) == typeid(policy::row_major)) {
      for (sz_t i = 0; i < size_x; i++) {
        for (sz_t j = 0; j < size_y; j++) {
          _array.push_back(exp(i, j));
        }
      }
    } else {
      for (sz_t j = 0; j < size_y; j++) {
        for (sz_t i = 0; i < size_x; i++) {
          _array.push_back(exp(i, j));
        }
      }
    }
  }

  /**
   * @brief      Gives the dimensions of the matrix
   */
  decltype(auto) shape() const { return std::make_pair(size_x, size_y); }
  /**
   * @brief      function for getting processed data i.e. member _array which is
   *             processed under row_major or column_major;
   */
  decltype(auto) pcd_data() const { return _array; }
  /**
   * Operator () Overloading for getting the (i,j)th element
   */
  inline const T operator()(const std::size_t i, const std::size_t j) const {
    return pol(i, j, *this);
  }
  inline T &operator()(const std::size_t i, const std::size_t j) {
    return pol(i, j, *this);
  }

  /**
   * @brief      Oveloading operator << to use std:: cout
   */
  friend std::ostream &operator<<(std::ostream &out,
                                  lazy_matrix<T, ploy> &other) {
    sz_t size_x = other.shape().first;
    sz_t size_y = other.shape().second;
#pragma omp parallel for
    for (sz_t i = 0; i < size_x; i++) {
      for (sz_t j = 0; j < size_y; j++) {
        out << other(i, j) << ' ';
      }
      out << std::endl;
    }
    return out;
  }

  /**
   * @brief      Overloading operator = for a assignment of lazy matrix
   *
   * @param[in]  other  reference to the matrix or expression which is to be
   *                    assigned
   *
   * @tparam     R1     expression type or matrix type
   */
  template <typename R1> lazy_matrix operator=(const R1 &other) {
    assert(shape() == other.shape());
    lazy_matrix<T, ploy> temp(size_x, size_y);
#pragma omp parallel for
    for (sz_t i = 0; i < size_x; i++) {
      for (sz_t j = 0; j < size_y; j++) {
        temp(i, j) = other(i, j);
      }
    }
#pragma omp parallel for
    for (sz_t i = 0; i < size_x; i++) {
      for (sz_t j = 0; j < size_y; j++) {
        (*this)(i, j) = temp(i, j);
      }
    }
    return *this;
  }

  /**
   * @brief      Overloading operator == for a comparing equality with other
   *             matrix
   *
   * @param[in]  other  reference to the matrix or expression which is to be
   *                    compared
   *
   * @tparam     R2     matrix or expression
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
   * @param[in]  other  reference to the matrix or expression which is to be
   *                    added
   *
   * @tparam     R1     matrix or expression
   */
  template <typename R1> decltype(auto) operator+(const R1 &other) {
    assert(shape() == other.shape());
    return expr<lazy_matrix<T, ploy>, R1, _add>(*this, other, _add());
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
   * @param[in]  other  reference to the matrix or expression which is to be
   *                    subtracted
   *
   * @tparam     R1     matrix or expression
   */
  template <typename R1> decltype(auto) operator-(const R1 &other) {
    assert(shape() == other.shape());
    return expr<lazy_matrix<T, ploy>, R1, _sub>(*this, other, _sub());
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
   * @tparam     R1     matrix or expression
   */
  template <typename R1> decltype(auto) operator/(const R1 &other) {
    assert(shape() == other.shape());
    return expr<lazy_matrix<T, ploy>, R1, _ediv>(*this, other, _ediv());
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
   * @tparam     R1     matrix or expression
   */
  template <typename R1> decltype(auto) operator*(const R1 &other) {
    assert(shape() == other.shape());
    return expr<lazy_matrix<T, ploy>, R1, _emul>(*this, other, _emul());
  }
  /**
   * @brief      assignment after element-wise multiplication
   */
  template <typename R1> void operator*=(const R1 &other) {
    auto temp = (*this) * other;
    *this = temp;
  }
  /**
   * @brief      Operator -%Overloading for Standard Matrix Multiplication
   *
   * @param[in]  other  reference to the matrix or expression which is to be
   *                    multiplied
   *
   * @tparam     R1     matrix or expression
   */
  template <typename R1> decltype(auto) operator%(const R1 &other) {
    assert(shape().second == other.shape().first);
    return expr<lazy_matrix<T, ploy>, R1, _std_mul>(*this, other, _std_mul());
  }
  /**
   * @brief      assignment after standard matrix multiplication
   */
  template <typename R1> void operator%=(const R1 &other) {
    auto temp = (*this) % other;
    *this = temp;
  }
};