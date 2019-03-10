#include "benchmark.h"
#include <iostream>
using namespace std;
int main() {
  benchmark<double> test(1000, 1000, 1.0);
  decltype(auto) result = test.run();
  test.print(result.first, result.second);
  lazy_matrix<int> a(2, 2, 2);
  auto c = a % a % a % a;
  lazy_matrix<decltype(c(0,0))> b= c;
  cout << b << endl;
  system("pause");
}
