#include <Include/testbench.h>

void Testbench::test(){
  wait();
  print();
}


void Testbench::print(){
  std::cout << "data output: " << data_in.read() << "\n";
}
