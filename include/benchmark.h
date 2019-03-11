#pragma once
#include "lazy_matrix.h"
#include "trad_matrix.h"
#include <chrono>
#include <fstream>
using _hrc = std::chrono::high_resolution_clock;
using _ms = std::chrono::milliseconds;
template <typename T> using _cd = std::chrono::duration<T>;
/**
 * @brief      Class for testing the trad_matrix and lazy_matrix
 *
 * @tparam     F     Type of matrix
 */
template <typename F> class benchmark {
private:
  lazy_matrix<F> lm;
  trad_matrix<F> tm;
  const sz_t size_x;
  const sz_t size_y;
  const F value;

public:
  benchmark(const sz_t &n, const sz_t &m)
      : lm(n, m, 2.0), tm(n, m, 2.0), size_x(n), size_y(m) {}
  benchmark(const sz_t &n, const sz_t &m, const F &val)
      : lm(n, m, val), tm(n, m, val), size_x(n), size_y(m), value(val) {}

  template <typename T> decltype(auto) test(T &a) {
    auto t1 = _hrc::now();
    a += a + a / a - a / a + a * a + a - a * a - a * a - a + a + a - a / a -
         a / a / a / a + a + a + a + a - a / a / a * a * a - a * a + a +
         a * a / a / a * a / a - a / a + a + a * a / a + a / a * a / a - a -
         a / a / a - a - a / a / a - a * a / a - a * a + a - a * a * a * a + a -
         a / a * a / a - a / a - a / a - a - a - a - a - a / a + a / a * a -
         a / a - a * a - a / a + a - a / a * a * a * a / a * a - a * a + a - a +
         a * a / a + a * a + a + a * a * a - a - a / a * a + a - a * a + a +
         a * a * a * a - a * a + a - a + a * a * a + a * a + a * a * a +
         a / a * a - a + a - a * a - a * a + a / a + a + a + a / a / a * a + a +
         a - a / a * a - a * a + a + a / a / a - a - a + a + a - a + a * a * a -
         a / a * a * a * a * a / a - a / a / a - a * a + a * a + a - a - a +
         a * a - a - a - a + a - a / a + a + a * a * a * a + a + a / a / a - a -
         a / a / a / a * a - a - a + a + a * a + a / a * a + a + a * a * a + a +
         a - a - a / a / a - a / a / a / a - a * a - a + a + a * a * a / a / a -
         a * a / a + a / a + a + a + a - a + a - a - a - a - a - a * a -
         a / a * a / a - a / a * a + a * a + a / a * a - a / a - a + a / a * a -
         a * a * a * a / a / a + a + a + a / a / a + a - a + a + a - a -
         a / a / a + a - a - a / a;
    auto t2 = _hrc::now();
    auto time_span = std::chrono::duration_cast<_ms>(_cd<double>(t2 - t1));
    return time_span;
  }
  template <typename R1> void print_file(R1 &result1, R1 &result2) {
    std::fstream fs;
    fs.open("D:/CodeFiles/GIT/uBLAS-Programming-Competency-Test/other/test_logs.txt", std::fstream::app);
    fs << std::endl;
    fs << "Results for matrix(" << size_x << " , " << size_y << " , " << value
       << ") :----------------" << std::endl;
    fs << "LAZY Matrix took       -------------- " << result1.count()
       << " ms\n";
    fs << "TRADITION Matrix took  -------------- " << result2.count()
       << " ms\n";
    fs.close();
  }
  template <typename R1> void print(R1 &result1, R1 &result2) {
    std::cout << std::endl;
    std::cout << "Results for matrix(" << size_x << " , " << size_y << " , "
              << value << ") :----------------" << std::endl;
    std::cout << "LAZY Matrix took       -------------- " << result1.count()
              << " ms\n";
    std::cout << "TRADITION Matrix took  -------------- " << result2.count()
              << " ms\n";
  }
  decltype(auto) run() {
    decltype(auto) rst1 = test(lm);
    decltype(auto) rst2 = test(tm);
    std::cout << (lm == tm) << std::endl;
    return std::make_pair(rst1, rst2);
  }
};
