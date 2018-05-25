#include <Include/testbench.h>

void Testbench::test(){
  std::cout<<"\t\t___________________________________________________"<<endl<<endl;
  std::cout << "\t" << "CLOCK || R_W (Read Write) | DIR_D (Direccion dato) \
               || DOUT (Dato a escribir) ||RA (Valor leido en direccion A)\n\n\n";
  std::cout << "\t\t" << "CLOCK" << " || " << "R_W " << " | " << "DIR_D" << "  || " << "DOUT" << " | " << "RA |\n";


  enable_out.write(1);
  read_write_out.write(1);
  dirdata_out.write(0);
  data_out.write(10);
  wait();
  print();

  enable_out.write(1);
  read_write_out.write(1);
  dirdata_out.write(1);
  data_out.write(20);
  wait();
  print();

  enable_out.write(1);
  read_write_out.write(1);
  dirdata_out.write(2);
  data_out.write(30);
  wait();
  print();


  enable_out.write(1);
  read_write_out.write(0);
  dirdata_out.write(0);
  data_out.write(10);
  wait();
  print();


  enable_out.write(1);
  read_write_out.write(0);
  dirdata_out.write(1);
  data_out.write(10);
  wait();
  print();


  enable_out.write(1);
  read_write_out.write(0);
  dirdata_out.write(2);
  data_out.write(10);
  wait();
  print();


  sc_stop();
}



void Testbench::print(){
  std::cout << "\t\t" << sc_time_stamp() << " || " << read_write_out.read() << "\t   | " << dirdata_out.read() << "\t || " << data_out.read() << "   | " << ra_in.read() << "  | \n";
  //RECUERDEN LA OTRA MANERA DE IMPRIMIR LOS DATOS (PARA VERLOS COMO NUMEROS BINARIOS)
        std::cout<<"\t\t___________________________________________________"<<"\n";
}
