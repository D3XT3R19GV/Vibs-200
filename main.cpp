#include<register_file.h>
#include<testbench.h>

int sc_main(int /*unused*/,char ** /*unused*/){
  sc_time DELAY(10,SC_NS);
  sc_time PERIOD(10,SC_NS);

  sc_clock clk("clock",PERIOD,0.5,DELAY,true);
  sc_signal< sc_uint<4> > dird, dira, dirb;
  sc_signal< sc_uint<32> > da, db, din;
  sc_signal< bool > enable;
  Register_File registerfile("registerfile");
  Testbench test("test");

  registerfile._Write_Enable(enable);
  registerfile._Write_Dir(dird);
  registerfile._Reg_A(dira);
  registerfile._Reg_B(dirb);
  registerfile._Write_Data(din);
  registerfile._Dat_A(da);
  registerfile._Dat_B(db);
  registerfile._Clock(clk);


  test.write_enable_out(enable);
  test.dirdata_out(dird);
  test.dira_out(dira);
  test.dirb_out(dirb);
  test.data_out(din);
  test.ra_in(da);
  test.rb_in(db);
  test.clk(clk);


  sc_start();

  return 0;
}
