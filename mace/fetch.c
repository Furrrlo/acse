/*
 * Giovanni Agosta, Andrea Di Biagio
 * Politecnico di Milano, 2007
 *
 * fetch.c
 * Formal Languages & Compilers Machine, 2007/2008
 *
 */
#include <stdlib.h>
#include <limits.h>
#include "decode.h"
#include "fetch.h"
#include "machine.h"

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define POSITIVE(X) ((X) >= 0 ? 1 : 0)
#define MSB(X) (((X) >> 31) & 1)


static int executeTER(decoded_instr *instr);
static int executeBIN(decoded_instr *instr);
static int executeUNR(decoded_instr *instr);
static int executeJMP(decoded_instr *instr);
static int handle_special_instruction(decoded_instr *instr);
static int perform_add(int a, int b, int *carry, int *overflow);
static int perform_sub(int a, int b, int *carry, int *overflow);
static int perform_shl(int value, int amount, int *carry);
static int perform_shr(int is_unsigned, int value, int amount, int *carry);
static int perform_rotl(int value, int amount, int *carry);
static int perform_rotr(int value, int amount, int *carry);

/* returns next pc, negative values are error codes, 0 is correct termination */
int fetch_execute(int pc)
{
   int result;
   decoded_instr *instr = decode(mem[pc]);
   switch (instr->format) {
      case TER: result = executeTER(instr); break;
      case BIN: result = executeBIN(instr); break;
      case UNR: result = executeUNR(instr); break;
      case JMP: result = executeJMP(instr); break;
      default: result = INVALID_INSTR_FORMAT;
   }

   /* free the memory associated with `instr' */
   if (instr != NULL)
      free(instr);

   /* return the new PC (or the value _HALT) as result */
   return result;
}

int executeTER(decoded_instr *instr)
{
   int *dest, *src1, *src2;
   int old_src1, old_src2;
   int carryout = 0, overflow = 0, negative = 0, zero = 0;
   long long int mulresult;

   /* Handle addressing modes (direct/indirect) */
   if (func_indirect_dest(instr))
      dest = &(mem[reg[instr->dest]]);
   else
      dest = &(reg[instr->dest]);
   src1 = &(reg[instr->src1]);
   if (func_indirect_src2(instr))
      src2 = &(mem[reg[instr->src2]]);
   else
      src2 = &(reg[instr->src2]);

   old_src1 = *src1;
   old_src2 = *src2;

   switch (instr->opcode) {
      case ADD:
         *dest = perform_add(*src1, *src2, &carryout, &overflow);
         if (func_carry(instr) && getflag(CARRY))
            *dest = perform_add(*dest, 1, &carryout, &overflow);
         break;
      case SUB:
         *dest = perform_sub(*src1, *src2, &carryout, &overflow);
         if (func_carry(instr) && getflag(CARRY))
            *dest = perform_sub(*src1, 1, &carryout, &overflow);
         break;
      case ANDL: *dest = *src1 && *src2; break;
      case ORL: *dest = *src1 || *src2; break;
      case EORL: *dest = (*src1 && !*src2) || (!*src1 && *src2); break;
      case ANDB: *dest = *src1 & *src2; break;
      case ORB: *dest = *src1 | *src2; break;
      case EORB: *dest = *src1 ^ *src2; break;
      case MUL:
         if (!func_is_unsigned(instr)) {
            mulresult = (long long)*src1 * (long long)*src2;
            if (mulresult < -(0x80000000LL) || mulresult > 0x7FFFFFFFLL)
               overflow = 1;
         } else {
            mulresult = (unsigned long long)*src1 * (unsigned long long)*src2;
            if ((unsigned long long)mulresult >= (0x100000000ULL))
               overflow = 1;
         }
         *dest = mulresult & UINT_MAX;
         if (func_carry(instr) && getflag(CARRY))
            *dest = perform_add(*dest, 1, &carryout, &overflow);
         break;
      case DIV:
         if (!func_is_unsigned(instr)) {
            if (old_src1 == INT_MIN && old_src2 == -1) {
               /* INT_MIN / -1 throws SIGFPE on x86_64 */
               overflow = 1;
               *dest = INT_MIN;
            } else {
               *dest = *src1 / *src2;
            }
         } else {
            *dest = ((unsigned)*src1) / ((unsigned)*src2);
         }
         if (func_carry(instr) && getflag(CARRY))
            *dest = perform_sub(*dest, 1, &carryout, &overflow);
         break;
      case SHL:
         *dest = perform_shl(old_src1, old_src2, &carryout);
         if (func_carry(instr) && getflag(CARRY))
            *dest = perform_add(*dest, 1, &carryout, &overflow);
         break;
      case SHR:
         *dest = perform_shr(func_is_unsigned(instr), old_src1, old_src2, &carryout);
         if (func_carry(instr) && getflag(CARRY))
            *dest = perform_add(*dest, 1, &carryout, &overflow);
         break;
      case ROTL: *dest = perform_rotl(*src1, *src2, &carryout); break;
      case ROTR: *dest = perform_rotr(*src1, *src2, &carryout); break;
      case NEG:
         *dest = perform_sub(0, *src2, &carryout, &overflow);
         if (func_carry(instr) && getflag(CARRY))
            *dest = perform_sub(*dest, 1, &carryout, &overflow);
         break;
      case SPCL: pc = handle_special_instruction(instr); break;
      default: return INVALID_INSTR;
   }
   zero = (*dest == 0);
   negative = (*dest < 0);

   setflag(CARRY, carryout);
   setflag(OVERFLOW, overflow);
   setflag(ZERO, zero);
   setflag(NEGATIVE, negative);

   return pc + 1;
}

int executeBIN(decoded_instr *instr)
{
   int *dest, *src1, *src2;
   int old_src1, old_src2;
   int carryout = 0, overflow = 0, negative = 0, zero = 0;
   long long int mulresult;

   /* Handle addressing modes (direct only) */
   dest = &(reg[instr->dest]);
   src1 = &(reg[instr->src1]);
   src2 = &(instr->imm);

   old_src1 = *src1;
   old_src2 = *src2;

   switch (instr->opcode) {
      case ADDI: *dest = perform_add(*src1, *src2, &carryout, &overflow); break;
      case SUBI: *dest = perform_sub(*src1, *src2, &carryout, &overflow); break;
      case ANDLI: *dest = *src1 && *src2; break;
      case ORLI: *dest = *src1 || *src2; break;
      case EORLI: *dest = (*src1 && !*src2) || (!*src1 && *src2); break;
      case ANDBI: *dest = *src1 & *src2; break;
      case ORBI: *dest = *src1 | *src2; break;
      case EORBI: *dest = *src1 ^ *src2; break;
      case MULI:
         mulresult = (long long)*src1 * (long long)*src2;
         if (mulresult < -(0x80000000LL) || mulresult > 0x7FFFFFFFLL)
            overflow = 1;
         *dest = mulresult & UINT_MAX;
         break;
      case DIVI:
         if (old_src1 == INT_MIN && old_src2 == -1) {
            *dest = INT_MIN;
            overflow = 1;
         } else {
            *dest = *src1 / *src2;
         }
         break;
      case SHLI: *dest = perform_shl(old_src1, old_src2, &carryout); break;
      case SHRI: *dest = perform_shr(0, old_src1, old_src2, &carryout); break;
      case ROTLI: *dest = perform_rotl(*src1, *src2, &carryout); break;
      case ROTRI: *dest = perform_rotr(*src1, *src2, &carryout); break;
      case NOTL: *dest = !*src1; break;
      case NOTB: *dest = ~*src1; break;
      default: return INVALID_INSTR;
   }

   zero = (*dest == 0);
   negative = (*dest < 0);

   setflag(CARRY, carryout);
   setflag(OVERFLOW, overflow);
   setflag(ZERO, zero);
   setflag(NEGATIVE, negative);

   return pc + 1;
}

int executeUNR(decoded_instr *instr)
{
   int *dest, src, new_psw;

   /* Handle addressing modes (direct only) */
   dest = &reg[instr->dest];
   src = instr->addr;

   /* default PC updating behavior */
   pc = pc + 1;

   switch (instr->opcode) {
      case NOP: /* NOP */ break;
      case HALT: /* HALT */ return _HALT; break;
      case MOVA:
         *dest = src; /* Move a 20-bit constant to a register */
         break;
      case LOAD: *dest = mem[src]; break;
      case STORE: mem[src] = *dest; break;
      case JSR:
         mem[--(*dest)] = pc; /* push next PC to the stack */
         pc = src;            /* jump to the address */
         break;
      case RET:
         pc = mem[(*dest)++]; /* pop the PC from the stack */
         break;
      case SEQ:
         *dest = getflag(ZERO);
         setflag(ZERO, (*dest == 0));
         setflag(NEGATIVE, 0);
         setflag(CARRY, 0);
         setflag(OVERFLOW, 0);
         break;
      case SGT:
         *dest = ((getflag(NEGATIVE) && getflag(OVERFLOW) &&
                        (!getflag(ZERO))) ||
               (!getflag(NEGATIVE) && !getflag(OVERFLOW) && !getflag(ZERO)));
         setflag(ZERO, (*dest == 0));
         setflag(NEGATIVE, 0);
         setflag(CARRY, 0);
         setflag(OVERFLOW, 0);
         break;
      case SGE:
         *dest = ((getflag(NEGATIVE) && getflag(OVERFLOW)) ||
               (!getflag(NEGATIVE) && !getflag(OVERFLOW)));
         setflag(ZERO, (*dest == 0));
         setflag(NEGATIVE, 0);
         setflag(CARRY, 0);
         setflag(OVERFLOW, 0);
         break;
      case SLE:
         *dest = (getflag(ZERO) || (getflag(NEGATIVE) && !getflag(OVERFLOW)) ||
               (!getflag(NEGATIVE) && getflag(OVERFLOW)));
         setflag(ZERO, (*dest == 0));
         setflag(NEGATIVE, 0);
         setflag(CARRY, 0);
         setflag(OVERFLOW, 0);
         break;
      case SLT:
         *dest = ((getflag(NEGATIVE) && !getflag(OVERFLOW)) ||
               (!getflag(NEGATIVE) && getflag(OVERFLOW)));
         setflag(ZERO, (*dest == 0));
         setflag(NEGATIVE, 0);
         setflag(CARRY, 0);
         setflag(OVERFLOW, 0);
         break;
      case SNE:
         *dest = !getflag(ZERO);
         setflag(ZERO, (*dest == 0));
         setflag(NEGATIVE, 0);
         setflag(CARRY, 0);
         setflag(OVERFLOW, 0);
         break;
      case READ:
         fputs("int value? >", stdout);
         fscanf(stdin, "%d", dest);
         break;
      case WRITE: fprintf(stdout, "%d\n", (*dest)); break;
      case XPSW:
         new_psw = *dest & 0xF;
         *dest = psw;
         psw = new_psw;
         break;
      default: return INVALID_INSTR;
   }

   /* update the value of program counter */
   return pc;
}

int executeJMP(decoded_instr *instr)
{
   /*
    * Handles Bcc instructions
    * (see M68000 docs for an overview of the possible branches)
    */
   int src;
   int taken;

   /* initialize src */
   src = instr->addr;
   taken = 0;

   switch (instr->opcode) {
      case BT:
         if (1)
            taken = 1;
         break;
      case BF:
         if (0)
            taken = 1;
         break;
      case BHI:
         if (!(getflag(CARRY) || getflag(ZERO)))
            taken = 1;
         break;
      case BLS:
         if (getflag(CARRY) || getflag(ZERO))
            taken = 1;
         break;
      case BCC:
         if (!getflag(CARRY))
            taken = 1;
         break;
      case BCS:
         if (getflag(CARRY))
            taken = 1;
         break;
      case BNE:
         if (!getflag(ZERO))
            taken = 1;
         break;
      case BEQ:
         if (getflag(ZERO))
            taken = 1;
         break;
      case BVC:
         if (!getflag(OVERFLOW))
            taken = 1;
         break;
      case BVS:
         if (getflag(OVERFLOW))
            taken = 1;
         break;
      case BPL:
         if (!getflag(NEGATIVE))
            taken = 1;
         break;
      case BMI:
         if (getflag(NEGATIVE))
            taken = 1;
         break;
      case BGE:
         if (!(getflag(NEGATIVE) ^ getflag(OVERFLOW)))
            taken = 1;
         break;
      case BLT:
         if (getflag(NEGATIVE) ^ getflag(OVERFLOW))
            taken = 1;
         break;
      case BGT:
         if (!(getflag(ZERO) || (getflag(NEGATIVE) ^ getflag(OVERFLOW))))
            taken = 1;
         break;
      case BLE:
         if (getflag(ZERO) || (getflag(NEGATIVE) ^ getflag(OVERFLOW)))
            taken = 1;
         break;
      default: return INVALID_INSTR;
   }

   /* test if the branch is taken or not */
   if (taken)
      pc += src;
   else
      pc++;

   return pc;
}


int handle_special_instruction(decoded_instr *instr)
{
   /* here should be inserted code to handle special instructions
    * using the function bits
    */
   return INVALID_INSTR;
}


static int perform_add(int a, int b, int *carry, int *overflow)
{
   int result = a + b;
   if (POSITIVE(result) != POSITIVE(a) && POSITIVE(a) == POSITIVE(b))
      *overflow = 1;
   if (MSB(result) < MSB(a) + MSB(b)) {
      *carry = 1;
   }
   return result;
}

static int perform_sub(int a, int b, int *carry, int *overflow)
{
   int result = a - b;
   if (POSITIVE(result) != POSITIVE(a) && POSITIVE(a) != POSITIVE(b))
      *overflow = 1;
   if (MSB(result) > MSB(a) - MSB(b)) {
      *carry = 1;
   }
   return result;
}

static int perform_shl(int value, int amount, int *carry)
{
   int dest;
   if (amount > 31) {
      dest = 0;
      if (amount == 32)
         *carry = value & 1;
   } else if (amount > 0) {
      dest = value << amount;
      *carry = !!((unsigned long long)((unsigned)value) & (1ULL << (32 - amount)));
   } else {
      dest = value;
   }
   return dest;
}

static int perform_shr(int is_unsigned, int value, int amount, int *carry)
{
   int dest, orig_amount = amount;
   amount = MAX(0, MIN(amount, 31));
   dest = value >> amount;
   if (!is_unsigned && !POSITIVE(value)) {
      /* Arithmetic shift */
      dest |= ((1 << amount) - 1) << MAX(32 - amount, 0);
   } else if (is_unsigned) {
      /* Logic shift */
      dest &= (1 << MAX(32 - amount, 0)) - 1;
   }
   if (amount) {
      if (orig_amount > 31)
         *carry = (!is_unsigned && !POSITIVE(value));
      else
         *carry = !!(value & (1 << (amount - 1)));
   }
   return dest;
}

static int perform_rotl(int value, int amount, int *carry)
{
   int result;
   int amount_mask;
   amount &= 31;
   amount_mask = (1 << amount) - 1;
   result = value << amount;
   result |= (value >> (32 - amount)) & amount_mask;
   *carry = amount && (result & 1);
   return result;
}

static int perform_rotr(int value, int amount, int *carry)
{
   int result;
   int amount_mask;
   amount &= 31;
   amount_mask = (1 << amount) - 1;
   result = (unsigned)value >> amount;
   result |= amount > 0 ? (value & amount_mask) << (32 - amount) : 0;
   *carry = amount && MSB(result);
   return result;
}
