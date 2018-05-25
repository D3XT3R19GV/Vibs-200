#include <Include/testbench.h>

void Testbench::test(){
  std::cout<<"\t\t___________________________________________________"<<endl<<endl;
  std::cout << "\t" << "CLOCK || W_EN (Write enable) | DIR_D (Direccion para escribir) \
               || DOUT (Dato a escribir) | DA (Direccion A a leer) | DB (Direccion B a leer) \
               |RA (Valor leido en direccion A)|RB (Valor leido en direccion B)|" "\n\n\n";
  std::cout << "\t\t" << "CLOCK" << " || " << "W_EN" << " | " << "DIR_D" << "  || " << "DOUT" << " | " << "DA" << " | " << "DB |" << "RA |" << "RB |" "\n";


  write_enable_out.write(1);
  dirdata_out.write(0);
  data_out.write(10);
  dira_out.write(0); //Y su vez tambien pido los valores de los registros en la direccion a y b
  dirb_out.write(1);

  wait();
  print();

  write_enable_out.write(1);
  dirdata_out.write(1);
  data_out.write(20);
  dira_out.write(0);
  dirb_out.write(1);

  wait();
  print();

  write_enable_out.write(1);
  dirdata_out.write(2);
  data_out.write(30);
  dira_out.write(1);
  dirb_out.write(2);

  wait();
  print();

  write_enable_out.write(0);
  dirdata_out.write(3);
  data_out.write(10);
  dira_out.write(2);
  dirb_out.write(3);

  wait();
  print();

  write_enable_out.write(0);
  dirdata_out.write(3);
  data_out.write(10);
  dira_out.write(3);
  dirb_out.write(1);

  wait();
  print();

  write_enable_out.write(0);
  dirdata_out.write(3);
  data_out.write(10);
  dira_out.write(3);
  dirb_out.write(1);

  wait();
  print();

  sc_stop();
}



void Testbench::print(){
  std::cout << "\t\t" << sc_time_stamp() << " || " << write_enable_out.read() << "\t   | " << dirdata_out.read() << "\t || " << data_out.read() << "   | " << dira_out.read() << "  | " << dirb_out.read() << "  | " << ra_in.read() << " | " << rb_in.read() << "\n";
  //RECUERDEN LA OTRA MANERA DE IMPRIMIR LOS DATOS (PARA VERLOS COMO NUMEROS BINARIOS)
        std::cout<<"\t\t___________________________________________________"<<"\n";
}
