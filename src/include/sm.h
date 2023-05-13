#ifndef SM_H
#	define SM_H

#	include <stdint.h>
#	include <stddef.h>

#	ifndef SM_UNIT
#		define SM_UNIT uint32_t
#	endif /* SM_UNIT */

typedef SM_UNIT sm_unit_t;

#	ifndef SM_STACK_SIZE
#		define SM_STACK_SIZE 65536
#	endif /* !SM_STACK_SIZE */

typedef enum {
	OP_NIL = 0,
	OP_MV,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_CMP,
	OP_JA,
	OP_JZ,
	OP_JL,
	OP_JG,
	OP_JNZ,
	OP_JGE,
	OP_JLE,
	OP_PRN,
	OP_JB,
	OP_GB,
	OP_SWP,
	OP_SHR,
	OP_SHL,
	OP_AND,
	OP_XOR,
	OP_OR,
	OP_NOT,
	OP_INC,
	OP_DEC,
	OP_HLT,
	OP_PSH,
	OP_POP,
	OP_BSR,
	OP_NAND,
	OP_NXOR,
	OP_NOR,
	OP_SN,
	OP_KINDS,
} op_kind_t;

typedef enum {
	REG_A = 0,
	REG_B,
	REG_C,
	REG_D,
	REG_E,
	REG_F,
	REG_G,
	REG_KINDS,
} reg_t;

typedef enum {
	ATOM_REG,
	ATOM_NUM,
} atom_kind_t;

typedef struct {
	atom_kind_t type;
	sm_unit_t value;
} atom_t;

typedef struct {
	op_kind_t type;
	atom_t *argv;
} op_t;

typedef struct {
	size_t index;
	size_t length;
	op_t *operations;
} op_set_t;

#	ifndef SM_C
extern uint8_t op_argc[OP_KINDS];
extern sm_unit_t getval(atom_t a, sm_unit_t (*regs)[REG_KINDS]);
extern void eval(op_set_t ps, sm_unit_t (*regs)[REG_KINDS]);
extern void free_op(op_t op);
extern void free_op_set(op_set_t ops);
extern atom_t reg(reg_t r);
extern atom_t num(sm_unit_t n);
#	endif /* !SM_C */

#endif /* !SM_H */
