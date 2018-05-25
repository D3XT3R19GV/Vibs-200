#ifndef VIBS200_H
#define VIBS200_H

#include <systemc.h>
#include <alu.H>
#include <data_memory.h>
#include <instruction_memory.h>
#include <register_file.h>
#include <regs.H>

#define PC_LEN 4
#define OPCODE_LEN 16
#define DAT_LEN 32
#define OP_DIR_LEN 4

class Vibs200: public sc_module
{
public:
  sc_uint<PC_LEN> _PC;
  sc_in_clk _Clock;

  SC_CTOR (Vibs200)
  {
    SC_METHOD (next)
        sensitive << _Clock.pos();

    SC_METHOD (split)
        sensitive << to_splt_sig;

    SC_METHOD (checkDependency)
        sensitive << to_dependency_check_sig;

    _Register_File    = new Register_File     ("Register_File_Module");
    _Instr_Mem        = new Instruction_Memory("Instruction_Memory_Module");

    _Op_P1            = new regs<OP_LEN>      ("Opcode_P1");
//    _Op_P2            = new regs<OP_LEN>      ("Opcode_P2");
//    _Op_P3            = new regs<OP_LEN>      ("Opcode_P3");
//    _Op_P4            = new regs<OP_LEN>      ("Opcode_P4");
    _Val_A            = new regs<DAT_LEN>     ("Value_A_register");
    _Val_B            = new regs<DAT_LEN>     ("Value_B_register");
    _Offset           = new regs<DAT_LEN>     ("Offset_register");
    _Instruction      = new regs<OPCODE_LEN>  ("Instruction_Register");
    _Destination      = new regs<OP_DIR_LEN>  ("Destination_Register");
    _Next_Instruction = new regs<OPCODE_LEN>  ("Next_Instruction_Register");

    //### FETCH STAGE ##########################################################
    _Instr_Mem->_Dir_In(_PC);
    _Instr_Mem->_Ins_Out(ins_sig);
    _Instr_Mem->_Ins_Out_Next(next_ins_sig);

    _Instruction->clk_in(_Clock);
    _Instruction->data_in(ins_sig);
    _Instruction->enable_in(true);
    _Instruction->data_out(to_splt_sig);

    _Next_Instruction->clk_in(_Clock);
    _Next_Instruction->data_in(next_ins_sig);
    _Next_Instruction->enable_in(true);
    _Next_Instruction->data_out(to_dependency_check_sig);
    //### END FETCH STAGE ######################################################

    //### DECODE STAGE #########################################################
    _Register_File->_Clock(_Clock);
    _Register_File->_Write_Enable(writeback_enable_sig);
    _Register_File->_Write_Dir(writeback_dir_sig);
    _Register_File->_Write_Data(writeback_data_sig);
    _Register_File->_Reg_A(reg_a_dir);
    _Register_File->_Reg_B(reg_b_dir);
    _Register_File->_Dat_A(val_a_sig);
    _Register_File->_Dat_B(val_b_sig);

    _Val_A->clk_in(_Clock);
    _Val_A->data_in(val_a_sig);
    _Val_A->enable_in(true);
    _Val_A->data_out(val_to_alu);

    //### END DECODE STAGE #####################################################

    //### EXECUTE STAGE ########################################################


    //### END EXECUTE STAGE ####################################################

    //### MEMORY STAGE #########################################################


    //### END MEMORY STAGE #####################################################

    //### WRITE-BACK STAGE #####################################################


    //### END WRITE-BACK STAGE #################################################
  }

private:

  Instruction_Memory *_Instr_Mem;
  Register_File      *_Register_File;
  regs<OPCODE_LEN>   *_Instruction , *_Next_Instruction;
  regs<DAT_LEN>      *_Offset, *_Val_A, *_Val_B;
  regs<OP_DIR_LEN>   *_Destination, _Op_P1, _Op_P2, _Op_P3, _Op_P4;

  bool _Data_A_Dependency;
  bool _Data_B_Dependency;

  sc_signal<sc_uint<OPCODE_LEN>> ins_sig,
                                 next_ins_sig,
                                 to_splt_sig,
                                 to_dependency_check_sig;

  sc_signal<sc_uint<OP_DIR_LEN>> dest_sig,
                                 reg_a_dir,
                                 reg_b_dir,
                                 val_a_sig,
                                 val_b_sig,
                                 op_p1_sig,
//                                 op_p2_sig,
//                                 op_p3_sig,
//                                 op_p4_sig,
//                                 offset_sig,
                                 writeback_dir_sig,
                                 writeback_data_sig;

  sc_signal<sc_uint<DAT_LEN>>   val_to_alu;

  sc_signal<bool> writeback_enable_sig;

  void next()
  {
    _PC += 1;
  }

  void split()
  {
    op_p1_sig = to_splt_sig.read().range(15,12);
    reg_a_dir = to_splt_sig.read().range(11,8);
    reg_b_dir = to_splt_sig.read().range(7,4);
    dest_sig  = to_splt_sig.read().range(3,0);
  }

  void checkDependency()
  {
    sc_signal<OP_DIR_LEN> check_sig;
    check_sig = to_dependency_check_sig.read().range(11,8);

    if (check_sig.read() == reg_a_dir.read())
      _Data_A_Dependency = true;
    else
      _Data_A_Dependency = false;

    check_sig = to_dependency_check_sig.read().range(7,4);

    if (check_sig.read() == reg_b_dir.read())
      _Data_B_Dependency = true;
    else
      _Data_B_Dependency = false;
  }
};

#endif // VIBS200_H
