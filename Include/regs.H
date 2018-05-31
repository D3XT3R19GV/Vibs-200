#ifndef regs_H
#define regs_H

#include <systemc.h>
#include <iostream>

template <int siz=4>
class regs : public sc_module
{
public:

  sc_in<bool> clk_in, enable_in;
  sc_in<sc_uint<siz> > data_in;
  sc_out<sc_uint<siz> > data_out;
  sc_uint<siz> data;
  
  SC_CTOR(regs)
  {
    data = 0;

    SC_METHOD(reading_data)
        sensitive << data_in << enable_in << clk_in.neg() ;

    SC_METHOD(writing_data)
        sensitive << clk_in.pos();
  }

private:
  void reading_data()
  {
    if (enable_in.read() == true)
    {
      data = data_in.read();
    }
  }

   void writing_data()
   {
     data_out.write(data);
   }
};

#endif
