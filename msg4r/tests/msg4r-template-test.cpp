#include <iostream>

template<typename T>
struct my_struct {
  my_struct(); 
  T& operation(T&);
  int field;
};

template<typename T>
my_struct<T>::my_struct(): field() {}

template<typename T>
T& my_struct<T>::operation(T& t) {
  std::cout << t << std::endl;
  return t;
}

template<>
double& my_struct<double>::operation(double& t) {
  std::cout << (t + 1) << std::endl;
  return t;
};

int main(int argc, char* argv[]) {
  int t1 = 1234;
  double t2 = 1.234;
  my_struct<int> my1;
  my_struct<double> my2;
  my1.operation(t1);
  my2.operation(t2);
  return 0;
}
