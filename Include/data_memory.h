#ifndef DATA_MEMORY_H
#define DATA_MEMORY_H

#include <systemc.h>
#define DIR_LEN 4
#define OP_LEN 32
#define MEM_SIZE 1 << (DIR_LEN-1)

class Data_Memory: public sc_module
{
public:

  // Clock signal that defines actions per cycle
  sc_in_clk _Clock;

  // Data related I/O ports
  sc_in<sc_uint<OP_LEN>> _Data_In;
  sc_in<sc_uint<DIR_LEN>> _Data_Dir;
  sc_out<sc_uint<OP_LEN>> _Data_Out;

  // Control Input-Only Ports
  sc_in<bool> _Read_Write;
  sc_in<bool> _Enable;


  SC_CTOR(Data_Memory)
  {
    // read method sensitive to negative threshold
    SC_METHOD (read)
        sensitive << _Clock.pos();

    // read method sensitive to positive threshold
    SC_METHOD (write)
        sensitive << _Clock.neg();
  }

private:

  // Data Memoey Array
  sc_uint<OP_LEN> _Memory[MEM_SIZE];

  // Read operation
  void read()
  {
    if (_Enable.read() == true && _Read_Write.read() == false)
      _Data_Out.write(_Memory[_Data_Dir.read()]);
  }

  // Write Operation
  void write()
  {
    if (_Enable.read() == true && _Read_Write.read() == true)
      _Memory[_Data_Dir.read()] = _Data_In.read();
  }
};


#endif // DATA_MEMORY_H
