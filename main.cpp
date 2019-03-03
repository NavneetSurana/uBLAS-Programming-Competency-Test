#include <iostream>
#include "matrix.hpp"
using namespace std;
int main() {
    matrix<int> a(10, 10, 2);
    // cout << a << endl;
    auto c = a + a + a + a + a + a + a + a + a + a;
    // cout << c << endl;
    auto d = a - a - a - a - a - a - a - a - a - a;
    // cout << d << endl;
    a += a + a;
    //cout << a << endl;
    a -= a + a;
    cout << a << endl;
}