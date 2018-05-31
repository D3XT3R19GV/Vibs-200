#define DEBUG
#include <iostream>
#include <systemc.h>
#include <vibs-200.h>
#include <testbench.h>

int sc_main(int/**/ , char **/**/)
{
  sc_time DELAY(10,SC_NS);
  sc_time PERIOD(10,SC_NS);

  sc_clock clk("clock",PERIOD,0.5,DELAY,true);
  sc_signal<sc_uint<DAT_LEN>> output;


  Vibs200 mic("micro");
  mic._Size = 6;
  mic._Clock(clk);
  mic.output(output);

  Testbench test("test");
  test.data_in(output);
  test.clk(clk);

  sc_start();
  return 0;
}
