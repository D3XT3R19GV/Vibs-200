#ifndef VIBS200_H
#define VIBS200_H

#include <systemc.h>
#include <alu.H>
#include <data_memory.h>
#include <instruction_memory.h>
#include <register_file.h>
#include <regs.H>
#include <mux.h>

#define PC_LEN 4
#define OPCODE_LEN 16
#define DAT_LEN 32
#define OP_DIR_LEN 4

#define LW 16
#define SW 17
#define DEBUG
class Vibs200: public sc_module
{
public:
  sc_uint<PC_LEN> _Size;
  sc_in_clk _Clock;
  sc_out<sc_uint<DAT_LEN>> output;

  SC_CTOR (Vibs200)
  {
    SC_METHOD (next)
        sensitive << _Clock.neg();

    SC_METHOD (split)
        sensitive << to_splt_sig;

//    SC_METHOD (checkDependency)
//        sensitive << to_dependency_check_sig;
    SC_METHOD (checkAluMux)
        sensitive << val_b_to_mux << offset_sig << op_p2_sig;
    SC_METHOD (checkMemMux)
        sensitive << mem_to_mux << alu_result_to_mux << op_p4_sig;
    SC_METHOD (print_output)
        sensitive << _Clock.pos();
    _PC = 0;

    _Alu              = new alu                  ("ALU");
    _Instr_Mem        = new Instruction_Memory   ("Instruction_Memory_Module");
    _Data_Memory      = new Data_Memory          ("Data_Memory_Module");
    _Register_File    = new Register_File        ("Register_File_Module");

    _Op_P1            = new regs<OP_DIR_LEN>         ("Opcode_P1");
    _Op_P2            = new regs<OP_DIR_LEN>         ("Opcode_P2");
    _Op_P3            = new regs<OP_DIR_LEN>         ("Opcode_P3");

    _Mem              = new regs<DAT_LEN>        ("Men_reg");
    _Val_A            = new regs<DAT_LEN>        ("Value_A_register");
    _Val_B            = new regs<DAT_LEN>        ("Value_B_register");
    _Val_B2           = new regs<DAT_LEN>        ("Value_B2_register");
    _Offset           = new regs<OP_DIR_LEN>     ("Offset_register");
    _Alu_Res          = new regs<DAT_LEN>        ("alu_result1");
    _Alu_Res2          = new regs<DAT_LEN>        ("alu_result2");
    _Instruction      = new regs<OPCODE_LEN>     ("Instruction_Register");
    _Destination1     = new regs<OP_DIR_LEN>     ("Destination_Register1");
    _Destination2     = new regs<OP_DIR_LEN>     ("Destination_Register2");
    _Destination3     = new regs<OP_DIR_LEN>     ("Destination_Register3");
    _Next_Instruction = new regs<OPCODE_LEN>     ("Next_Instruction_Register");

    _Data_Mux         = new mux<DAT_LEN,DAT_LEN> ("data_mux");
    _Offset_Mux       = new mux<DAT_LEN,PC_LEN>  ("offset_mux");

    regs_enables.write(true);
    //### FETCH STAGE ##########################################################
    _Instr_Mem->_Dir_In(pc_sig);
    _Instr_Mem->_Ins_Out(ins_sig);
    _Instr_Mem->_Ins_Out_Next(next_ins_sig);

    _Instruction->clk_in(_Clock);
    _Instruction->data_in(ins_sig);
    _Instruction->enable_in(regs_enables);
    _Instruction->data_out(to_splt_sig);

    _Next_Instruction->clk_in(_Clock);
    _Next_Instruction->data_in(next_ins_sig);
    _Next_Instruction->enable_in(regs_enables);
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
    _Val_A->enable_in(regs_enables);
    _Val_A->data_out(val_a_to_alu);

    _Val_B->clk_in(_Clock);
    _Val_B->data_in(val_b_sig);
    _Val_B->enable_in(regs_enables);
    _Val_B->data_out(val_b_to_mux);

    _Offset->clk_in(_Clock);
    _Offset->data_in(reg_a_dir);
    _Offset->enable_in(regs_enables);
    _Offset->data_out(offset_sig);

    _Destination1->clk_in(_Clock);
    _Destination1->data_in(dest_sig);
    _Destination1->enable_in(regs_enables);
    _Destination1->data_out(dest_sig2);

    _Op_P1->clk_in(_Clock);
    _Op_P1->data_in(op_p1_sig);
    _Op_P1->enable_in(regs_enables);
    _Op_P1->data_out(op_p2_sig);
    //### END DECODE STAGE #####################################################

    //### EXECUTE STAGE #######################################################
    _Offset_Mux->active(aux_ena);

    _Offset_Mux->long_in(val_b_to_mux);
    _Offset_Mux->short_in(offset_sig);
    _Offset_Mux->long_out(val_b_to_alu);

    _Alu->data_a_in(val_a_to_alu);
    _Alu->data_b_in(val_b_to_alu);
    _Alu->data_out(alu_result);
    _Alu->opcode_in(op_p2_sig);

    _Val_B2->clk_in(_Clock);
    _Val_B2->data_in(val_b_to_mux);
    _Val_B2->enable_in(regs_enables);
    _Val_B2->data_out(to_mem_data);

    _Destination2->clk_in(_Clock);
    _Destination2->data_in(dest_sig2);
    _Destination2->enable_in(regs_enables);
    _Destination2->data_out(dest_sig3);

    _Op_P2->clk_in(_Clock);
    _Op_P2->data_in(op_p2_sig);
    _Op_P2->enable_in(regs_enables);
    _Op_P2->data_out(op_p3_sig);


    _Alu_Res->clk_in(_Clock);
    _Alu_Res->data_in(alu_result);
    _Alu_Res->enable_in(regs_enables);
    _Alu_Res->data_out(alu_result2);
    //### END EXECUTE STAGE ####################################################

    //### MEMORY STAGE #########################################################
    _Data_Memory->_Clock(_Clock);
    _Data_Memory->_Data_In(to_mem_data);
    _Data_Memory->_Data_Out(to_mem_reg);
    _Data_Memory->_Data_Dir(alu_result2);

    _Data_Memory->_Enable(aux_ena1);


    _Data_Memory->_Read_Write(r_w);

    _Destination3->clk_in(_Clock);
    _Destination3->data_in(dest_sig3);
    _Destination3->enable_in(regs_enables);
    _Destination3->data_out(writeback_dir_sig);

    _Op_P3->clk_in(_Clock);
    _Op_P3->data_in(op_p3_sig);
    _Op_P3->enable_in(regs_enables);
    _Op_P3->data_out(op_p4_sig);

    _Alu_Res2->clk_in(_Clock);
    _Alu_Res2->data_in(alu_result2);
    _Alu_Res2->enable_in(regs_enables);
    _Alu_Res2->data_out(alu_result_to_mux);

    _Mem->clk_in(_Clock);
    _Mem->data_in(to_mem_reg);
    _Mem->enable_in(regs_enables);
    _Mem->data_out(mem_to_mux);
    //### END MEMORY STAGE #####################################################

    //### WRITE-BACK STAGE #####################################################
    _Data_Mux->active(aux_mux);
    _Data_Mux->long_in(alu_result_to_mux);
    _Data_Mux->short_in(mem_to_mux);
    _Data_Mux->long_out(writeback_data_sig);
    //### END WRITE-BACK STAGE #################################################
  }

public:
  alu                  *_Alu;
  mux<DAT_LEN,DAT_LEN> *_Data_Mux;
  Instruction_Memory   *_Instr_Mem;
  mux<DAT_LEN,PC_LEN>  *_Offset_Mux;
  Data_Memory          *_Data_Memory;
  Register_File        *_Register_File;
  regs<OPCODE_LEN>     *_Instruction , *_Next_Instruction;
  regs<DAT_LEN>        *_Val_A, *_Val_B, *_Val_B2, *_Alu_Res, *_Alu_Res2, *_Mem;
  regs<OP_DIR_LEN>     *_Offset, *_Destination1, *_Destination2, *_Destination3;
  regs<OP_DIR_LEN>     *_Op_P1, *_Op_P2, *_Op_P3;


  bool _Data_A_Dependency;
  bool _Data_B_Dependency;

  sc_signal<sc_uint<OPCODE_LEN>> ins_sig,
                                 next_ins_sig,
                                 to_splt_sig,
                                 to_dependency_check_sig;

  sc_signal<sc_uint<OP_DIR_LEN>> pc_sig,
                                 dest_sig,
                                 dest_sig2,
                                 dest_sig3,
                                 reg_a_dir,
                                 reg_b_dir,
                                 op_p1_sig,
                                 op_p2_sig,
                                 op_p3_sig,
                                 op_p4_sig,
                                 offset_sig,
                                 writeback_dir_sig;

  sc_signal<sc_uint<DAT_LEN>>   val_a_sig,
                                val_b_sig,
                                alu_result,
                                to_mem_reg,
                                mem_to_mux,
                                alu_result2,
                                to_mem_data,
                                val_a_to_alu,
                                val_b_to_alu,
                                val_b_to_mux,
                                alu_result_to_mux,
                                writeback_data_sig;


  sc_signal<bool> r_w,
                  aux_mux,
                  aux_ena,
                  aux_ena1,
                  regs_enables,
                  writeback_enable_sig;

  unsigned int _PC;

  void next()
  {
    if (_PC <= 11)//_Size + 5)
    {
      _PC += 1;
      pc_sig.write(_PC);
    }
    else
    {
      sc_stop();
    }
  }

  void split()
  {
    op_p1_sig = to_splt_sig.read().range(15,12);
    reg_a_dir = to_splt_sig.read().range(11,8);
    reg_b_dir = to_splt_sig.read().range(7,4);
    dest_sig  = to_splt_sig.read().range(3,0);

    std::cout << "opcode: ";
    for (int i=0; i < 4; ++i)
      std::cout  << op_p1_sig.read()[4 -1 -i] ;

    std::cout << "  reg a dir: ";
    for (int i=0; i < 4; ++i)
      std::cout  << reg_a_dir.read()[4 -1 -i] ;

    std::cout << "  reg b dir: ";
    for (int i=0; i < 4; ++i)
      std::cout  << reg_b_dir.read()[4 -1 -i] ;

    std::cout << "  dest: ";
    for (int i=0; i < 4; ++i)
      std::cout  << dest_sig.read()[4 -1 -i] ;

    std::cout << "\n";
  }

//  void checkDependency()
//  {
//    sc_signal<sc_uint <OP_DIR_LEN>> check_sig;
//    check_sig = to_dependency_check_sig.read().range(11,8);

//    if (check_sig.read() == reg_a_dir.read())
//      _Data_A_Dependency = true;
//    else
//      _Data_A_Dependency = false;

//    check_sig = to_dependency_check_sig.read().range(7,4);

//    if (check_sig.read() == reg_b_dir.read())
//      _Data_B_Dependency = true;
//    else
//      _Data_B_Dependency = false;
//  }

  void checkAluMux()
  {
    if(op_p2_sig.read() == SW || op_p2_sig.read() == LW )
      aux_ena.write(true);
    else
      aux_ena.write(false);
  }

  void checkMemMux()
  {
    if(op_p4_sig.read() == SW || op_p4_sig.read() == LW)
      aux_ena1.write(true);
    else
      aux_ena1.write(false);

    if(op_p4_sig.read() == SW )
      writeback_enable_sig.write(true);
    else
      writeback_enable_sig.write(false);
  }
  void print_output()
  {
    output.write(writeback_data_sig.read());
  }
};

#endif  //VIBS200_H
