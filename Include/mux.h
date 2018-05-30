#ifndef MUX_H
#define MUX_H

#include <systemc.h>

class mux: public sc_module
{
public:
  sc_in<bool> active;
  sc_in<sc_uint<32>> long_in;
  sc_in<sc_uint<4>> short_in;
  sc_out<sc_uint<32>> long_out;

  SC_CTOR(mux)
  {
    SC_METHOD(switch_mux)
        sensitive << long_in << short_in << active;
  }

private:
  void switch_mux()
  {
    if(active.read())
    {
      long_out.write(short_in.read());
      return;
    }
    long_out.write(long_in.read());
  }
};

#endif // MUX_H
