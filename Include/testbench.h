#ifndef TESTBENCH_H
#define TESTBENCH_H


#include <systemc.h>
#include <iostream>


class Testbench: public sc_module{

public:
  sc_in_clk clk;

  sc_out<bool> write_enable_out;
  sc_out<sc_uint<4> > dirdata_out;

  sc_out<sc_uint<4> > dira_out;

  sc_out<sc_uint<4> > dirb_out;

  sc_out<sc_uint<32> > data_out;

  sc_in<sc_uint<32> > ra_in;

  sc_in<sc_uint<32> > rb_in;

  SC_CTOR(Testbench){
    SC_THREAD(test);
    sensitive<<clk.neg();
    dont_initialize();
  }
private:
  void test();
  void print();
};

#endif // TESTBENCH_H
