/*
 * Giovanni Agosta, Andrea Di Biagio
 * Politecnico di Milano, 2007
 *
 * machine.h
 * Formal Languages & Compilers Machine, 2007/2008
 *
 */
#ifndef _MACHINE_H
#define _MACHINE_H

#define NREGS 32
#define MEMSIZE 4096

#define _HALT -1

#include <stdio.h>
#include "getbits.h"

/* FORMATS 0 to 3 */
enum formats { TER, BIN, UNR, JMP };

/* OPCODES (from 0 to 15) */
enum opcTER {
   ADD,
   SUB,
   ANDL,
   ORL,
   EORL,
   ANDB,
   ORB,
   EORB,
   MUL,
   DIV,
   SHL,
   SHR,
   ROTL,
   ROTR,
   NEG,
   SPCL
};
enum opcBIN {
   ADDI,
   SUBI,
   ANDLI,
   ORLI,
   EORLI,
   ANDBI,
   ORBI,
   EORBI,
   MULI,
   DIVI,
   SHLI,
   SHRI,
   ROTLI,
   ROTRI,
   NOTL,
   NOTB
};
enum opcUNR {
   NOP,
   MOVA,
   JSR,
   RET,
   LOAD,
   STORE,
   HALT,
   SEQ,
   SGE,
   SGT,
   SLE,
   SLT,
   SNE,
   READ,
   WRITE,
   XPSW
};
enum opcJMP {
   BT,
   BF,
   BHI,
   BLS,
   BCC,
   BCS,
   BNE,
   BEQ,
   BVC,
   BVS,
   BPL,
   BMI,
   BGE,
   BLT,
   BGT,
   BLE
};

/* Exit codes (OK = no error) */
enum term {
   OK,
   NOARGS,
   NOFILE,
   INVALID_INSTR,
   INVALID_INSTR_FORMAT,
   MEM_FAULT,
   WRONG_FORMAT,
   WRONG_ARGS,
   BREAK
};

/* Internal memory */
extern int reg[NREGS];

void print_regs(FILE *file);
void print_psw(FILE *file);

extern int mem[MEMSIZE];

void print_Memory_Dump(FILE *file, int begin);

extern unsigned int pc; /* the program counter */
extern int psw;         /* the four condition flags */

enum flags { CARRY, OVERFLOW, ZERO, NEGATIVE };

/* Get flag from processor status word */
unsigned int getflag(int flag);

/* Set flag in processor status word*/
void setflag(int flag, int value);

#endif /* _MACHINE_H */
