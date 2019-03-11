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
  lazy_matrix<int> a(2,2,2);
  trad_matrix<int> b(2,2,2);
  b=b%b%b;
  auto c=a%a%a;
  cout<< c<< endl;
  cout<< b<<endl;
}
