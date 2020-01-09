#include <stdio.h>
#include "cpu.h"
#include "r_instructions.h"
#include "i_instructions.h"
#include "j_instructions.h"

//global variables
unsigned int registers[32];
unsigned int pc;
unsigned int npc;
unsigned int HI;
unsigned int LO;
unsigned char memory[1<<20];

int exitloop;

//macro for extracting opcode fields
#define OPCODE(x) ((x>>26) & (0x3F))

//macro for exracting funct field of R INSTRUCTIONS
#define FUNCT(x) (x & (0x3f))

//R instructions
#define ADD_R (0x20)
#define ADDU_R (0x21)
#define SUB_R (0x22)
#define SUBU_R (0x23)
#define MULT_R (0x18)
#define MULTU_R (0x19)
#define DIV_R (0x1a)
#define DIVU_R (0x1b)
#define MFHI_R (0x10)
#define MFLO_R (0x12)
#define AND_R (0x24)
#define OR_R (0x25)
#define XOR_R (0x26)
#define NOR_R (0x27)
#define SLT_R (0x2a)
#define SLTU_R (0x2b)
#define SLL_R (0x00)
#define SLLV_R (0x04)
#define SRL_R (0x02)
#define SRLV_R (0x06)
#define SRA_R (0x03)
#define SRAV_R (0x07)
#define JR_R (0x08)
#define JALR_R (0x09)
#define SYSCALL_R (0x0c)

//I INSTRUCTIONS
#define ADDI_OPCODE (0x08)
#define ADDIU_OPCODE (0x09)
#define LW_OPCODE (0x23)
#define LH_OPCODE (0x21)
#define LHU_OPCODE (0x25)
#define LB_OPCODE (0x20)
#define LBU_OPCODE (0x24)
#define SW_OPCODE (0x2b)
#define SH_OPCODE (0x29)
#define SB_OPCODE (0x28)
#define LUI_OPCODE (0x0f)
#define ORI_OPCODE (0x0d)
#define ANDI_OPCODE (0x0c)
#define XORI_OPCODE (0x0e)
#define SLTI_OPCODE (0x0a)
#define SLTIU_OPCODE (0x0b)
#define BEQ_OPCODE (0x04)
#define BNE_OPCODE (0x05)

//J INSTRUCTIONS
#define JUMP_OPCODE (0x02)
#define JAL_OPCODE (0x03)

void cpu_initialize(){
  //Initialize both pc and npc to CODE_STARTING_ADDRESS.
  pc = CODE_STARTING_ADDRESS;
  npc = CODE_STARTING_ADDRESS;

  //Initialize reg 29 (stack pointer) to STACK_STARTING_ADDRESS, which is defined in cpu.h.
  registers[29] = STACK_STARTING_ADDRESS;

  //Set register 0 to zero (since register 0 should always contain zero).
  registers[0] = 0;
}

void handle_r_instructions(int instruction){
  int funct = FUNCT(instruction);
  switch(funct){
      case ADD_R: add(instruction);
        break;
      case ADDU_R: addu(instruction);
        break;
      case SUB_R: sub(instruction);
        break;
      case SUBU_R: subu(instruction);
        break;
      case MULT_R: mult(instruction);
        break;
      case MULTU_R: multu(instruction);
        break;
      case DIV_R: div(instruction);
        break;
      case DIVU_R: divu(instruction);
        break;
      case MFHI_R: mfhi(instruction);
        break;
      case MFLO_R: mflo(instruction);
        break;
      case AND_R: and(instruction);
        break;
      case OR_R: or(instruction);
        break;
      case XOR_R: xor(instruction);
        break;
      case NOR_R: nor(instruction);
        break;
      case SLT_R: slt(instruction);
        break;
      case SLTU_R: sltu(instruction);
        break;
      case SLL_R: sll(instruction);
        break;
      case SLLV_R: sllv(instruction);
        break;
      case SRL_R: srl(instruction);
        break;
      case SRLV_R: srlv(instruction);
        break;
      case SRA_R: sra(instruction);
        break;
      case SRAV_R: srav(instruction);
        break;
      case JR_R: jr(instruction);
        break;
      case JALR_R: jalr(instruction);
        break;
      case SYSCALL_R: syscall(instruction);
        break;
  }

}

void switch_statement(int instruction){
  int opcode = OPCODE(instruction);
  switch(opcode){
    case 0: handle_r_instructions(instruction);
      break;
    case ADDI_OPCODE: addi(instruction);
      break;
    case ADDIU_OPCODE: addiu(instruction);
      break;
    case LW_OPCODE: lw(instruction);
      break;
    case LH_OPCODE: lh(instruction);
      break;
    case LHU_OPCODE: lhu(instruction);
      break;
    case LB_OPCODE: lb(instruction);
      break;
    case LBU_OPCODE: lbu(instruction);
      break;
    case SW_OPCODE: sw(instruction);
      break;
    case SH_OPCODE: sh(instruction);
      break;
    case SB_OPCODE: sb(instruction);
      break;
    case LUI_OPCODE: lui(instruction);
      break;
    case ORI_OPCODE: ori(instruction);
      break;
    case ANDI_OPCODE: andi(instruction);
      break;
    case SLTI_OPCODE: slti(instruction);
      break;
    case SLTIU_OPCODE: sltiu(instruction);
      break;
    case BEQ_OPCODE: beq(instruction);
      break;
    case BNE_OPCODE: bne(instruction);
      break;
    case JUMP_OPCODE: j(instruction);
      break;
    case JAL_OPCODE: jal(instruction);
      break;
  }
}

void cpu_execute(){
  exitloop = 0;
  while(exitloop==0){
    pc=npc;
    npc = npc+4;
    unsigned int *instruction = (unsigned int *) &memory[pc];
    //int opcode = OPCODE(*instruction);
    switch_statement(*instruction);
  }
}

void cpu_exit(int errorcode){
  if(errorcode == 0){
    printf("The program terminated normally.");
  }
  else{
    printf("The program terminated with an error %d", errorcode);
  }
  exitloop = 1;
}
