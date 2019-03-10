/**
 * test execution file
 */
#include "include/benchmark.h"
#include <iostream>
using namespace std;
int main() {
  benchmark<double> test(1000, 1000, 1.0);
  decltype(auto) result = test.run();
  test.print(result.first, result.second);
}
