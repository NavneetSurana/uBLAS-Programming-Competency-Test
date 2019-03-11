/**
 * test execution file
 */
#include "D:/CodeFiles/GIT/uBLAS-Programming-Competency-Test/include/benchmark.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
int main() {
  ofstream out;
  out.open("D:/CodeFiles/GIT/uBLAS-Programming-Competency-Test/other/rsltGraph.xls");
  out<<"Size of Square Matrix\tLazy Matrix\tTraditional Matrix"<<endl;
  for (int i = 50; i <= 2000; i += 50) {
    benchmark<double> test(i, i, 1.0);
    decltype(auto) result = test.run();
    out << i << "\t" << result.first.count() << "\t" << result.second.count()
        << endl;
    test.print(result.first, result.second);
    test.print_file(result.first, result.second);
  }
  out.close();
}
