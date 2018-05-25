#include <data_memory.h>
#include <testbench.h>
#include <Vibs-200>
int sc_main(int /*unused*/,char ** /*unused*/){
  sc_time DELAY(10,SC_NS);
  sc_time PERIOD(10,SC_NS);

  sc_clock clk("clock",PERIOD,0.5,DELAY,true);
  sc_signal< sc_uint<4> > dird;
  sc_signal< sc_uint<32> > dout, din;
  sc_signal< bool > enable,rw;
  Data_Memory datamem("datamem");
  Testbench test("test");

  datamem._Clock(clk);
  datamem._Data_Dir(dird);
  datamem._Data_In(din);
  datamem._Data_Out(dout);
  datamem._Enable(enable);
  datamem._Read_Write(rw);

  test.read_write_out(rw);
  test.enable_out(enable);
  test.dirdata_out(dird);
  test.data_out(din);
  test.ra_in(dout);
  test.clk(clk);


  sc_start();

  return 0;
}
