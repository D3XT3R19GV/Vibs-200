#ifndef alu_H_
#define alu_H_

#include <systemc.h>

class alu : public sc_module
{
public:
  sc_in<sc_uint<4> > opcode_in;
  sc_in<sc_uint<32> > data_a_in;
  sc_in<sc_uint<32> > data_b_in;

  
  sc_out<sc_uint<32> > data_out;
                              
  SC_CTOR(alu)
  {
    SC_METHOD(operation);
    sensitive << data_a_in << data_b_in << opcode_in;
  }
  
private:
   void operation(){
      switch(opcode_in.read())
      {
	 case 0: //add
	    data_out.write(data_a_in.read() + data_b_in.read());
	    break;
	 case 1: //sub
	    data_out.write(data_a_in.read() - data_b_in.read());
	    break;
	 case 2: //mul
	    data_out.write(data_a_in.read() * data_b_in.read());
	    break;
	 case 3: //div
	    data_out.write(data_a_in.read() / data_b_in.read());
	    break;
	 case 4: //mod
	    data_out.write(data_a_in.read() % data_b_in.read());
	    break;
	 case 5: //max
	    if (data_a_in.read() > data_b_in.read())
	       data_out.write(data_a_in.read());
	    else 
	       data_out.write(data_b_in.read());
	    break;
	 case 6: //or
	    data_out.write(data_a_in.read() or data_b_in.read());
	    break;
	 case 7: //and
	    data_out.write(data_a_in.read() and data_b_in.read());  
	    break;
      }
   }
};

#endif
