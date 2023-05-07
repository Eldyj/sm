#ifndef SM_H
#	define SM_H

#	include <stdint.h>
#	include <stddef.h>

#	ifndef SM_UNIT
#		define SM_UNIT uint32_t
#	endif /* SM_UNIT */

typedef SM_UNIT sm_unit_t;

typedef enum {
	op_null = 0,
	op_mv,
	op_add,
	op_sub,
	op_mul,
	op_div,
	op_mod,
	op_cmp,
	op_ja,
	op_jz,
	op_jl,
	op_jg,
	op_jnz,
	op_jge,
	op_jle,
	op_prn,
	op_jb,
	op_gb,
	op_swp,
	op_shr,
	op_shl,
	op_and,
	op_xor,
	op_or,
	op_not,
	op_inc,
	op_dec,
	op_hlt,
	op_psh,
	op_pop,
} op_kind_t;

typedef enum {
	reg_a = 0,
	reg_b,
	reg_c,
	reg_d,
	reg_e,
	reg_f,
	reg_g,
	reg_kinds,
} reg_t;

typedef enum {
	atom_register,
	atom_number,
} atom_kind_t;

typedef struct {
	atom_kind_t type;
	sm_unit_t value;
} atom_t;

typedef struct {
	op_kind_t type;
	uint8_t argc;
	atom_t *argv;
} op_t;

typedef struct {
	size_t index;
	size_t length;
	op_t *operations;
} op_set_t;

#	ifndef SM_C
extern sm_unit_t getval(atom_t a, sm_unit_t (*regs)[reg_kinds]);
extern void eval(op_set_t ps, sm_unit_t (*regs)[reg_kinds]);
extern void free_op(op_t op);
extern void free_op_set(op_set_t ops);
extern atom_t reg(reg_t r);
extern atom_t num(sm_unit_t n);
#	endif /* !SM_C */

#endif /* !SM_H */
