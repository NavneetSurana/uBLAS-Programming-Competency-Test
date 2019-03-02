#include <iostream>
#include "matrix.hpp"
using namespace std;
int main() {
  matrix<int> a = {{2, 2}, {2, 2}};
  // cout << a << endl;
  auto c = a + a + a + a + a + a + a + a + a + a;
  cout << c << endl;
}