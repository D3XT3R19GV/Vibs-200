#ifndef TESTBENCH_H
#define TESTBENCH_H


#include <systemc.h>
#include <iostream>


class Testbench: public sc_module{

public:
  sc_in_clk clk;

  sc_in<sc_uint<32> > data_in;


  SC_CTOR(Testbench){
    SC_THREAD(test)
        sensitive<<clk.neg();
    dont_initialize();
  }
private:
  void test();
  void print();
};

#endif // TESTBENCH_H
