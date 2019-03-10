#include<iostream>
#include<ctime>
using namespace std;

int main() {
  srand(time(NULL));
  string temp;
  string a = "a";
  string op[] = {"+", "-", "*", "/"};
  cout << "a+= a" << ' ';
  for (int i = 1; i < 300; i++) {
    cout << op[rand() % 4] << ' ' << a << ' ';
  }
  cout << ";";
}