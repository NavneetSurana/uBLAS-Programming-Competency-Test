#include <iostream>
#include "matrix.hpp"
using namespace std;
int main() {
    matrix<int> a(10000,10000,100000);
    // cout << a << endl;
    auto c = a + a + a + a + a + a + a + a + a + a;
    //cout << c << endl;
    auto d = a - a - a - a - a - a - a - a - a - a;
    //cout << d << endl;
}