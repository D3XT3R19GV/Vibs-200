#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include <systemc.h>
#define DIR_LEN 4
#define OP_LEN 32
#define REG_FILE_SIZE 1 << (DIR_LEN-1)

class Register_File: public sc_module
{
public:

  // Clock signal that defines actions per cycle
  sc_in_clk _Clock;

  // Register Directions to be read
  sc_in<sc_uint<DIR_LEN>> _Reg_A;
  sc_in<sc_uint<DIR_LEN>> _Reg_B;

  // Register Data to be outputed
  sc_out<sc_uint<OP_LEN>> _Dat_A;
  sc_out<sc_uint<OP_LEN>> _Dat_B;

  // Signal to activate register write
  sc_in<bool> _Write_Enable;

  // Register direction to be written
  sc_in<sc_uint<DIR_LEN>> _Write_Dir;

  // Data to be written in _Write_Dir
  sc_in<sc_uint<OP_LEN>> _Write_Data;

  SC_CTOR (Register_File)
  {
    // read method sensitive to negative threshold
    SC_METHOD (read)
    sensitive << _Clock.pos();

    // read method sensitive to positive threshold
    SC_METHOD (write)
    sensitive << _Clock.neg();
  }

private:
  // Registers stored in an array
  sc_uint<OP_LEN> _Registers[REG_FILE_SIZE];

  // Read operation
  void read()
  {
    _Dat_A.write(_Registers[_Reg_A.read() ]);
    _Dat_B.write(_Registers[_Reg_B.read() ]);
  }

  // Write operation
  void write()
  {
    if (_Write_Enable.read() == true)
      _Registers[_Write_Dir.read()] = _Write_Data.read();
  }
};

#endif // REGISTER_FILE_H
