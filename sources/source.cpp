// Copyright 2018 Your Name <your_email>

#include <header.hpp>
#include <iostream>
using namespace std;
int main() {
  SharedPtr<int> p;
  SharedPtr<int> p2;
  int x = 50;
  p.reset(&x);
  int y = 50000;
  p2.reset(&y);
  cout << "P1:" << p.get() << endl;
  cout << "P2:" << p2.get() << endl;
  cout << "P1:" << p.use_count() << endl;
  cout << "P2:" << p2.use_count() << endl;
  p.Swap(p2);
  cout << "P1:" << p.get() << endl;
  cout << "P2:" << p2.get() << endl;
  p = p2;
  cout << "P1:" << p.use_count() << endl;
  cout << "P2:" << p2.use_count() << endl;
}
