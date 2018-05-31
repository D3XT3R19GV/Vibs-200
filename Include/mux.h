#ifndef MUX_H
#define MUX_H

#include <systemc.h>
template <int size1=32, int size2=4>
class mux: public sc_module
{
public:

  sc_in<bool> active;
  sc_in<sc_uint<size1>> long_in;
  sc_in<sc_uint<size2>> short_in;
  sc_out<sc_uint<size1>> long_out;

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
