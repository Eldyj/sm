#ifndef SBM_H
#	define SBM_H

#	include <stdio.h>
#	include <stdlib.h>

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
	unsigned long value;
} atom_t;

typedef struct {
	op_kind_t type;
	unsigned short argc;
	atom_t *argv;
} op_t;

typedef struct {
	unsigned long index;
	unsigned long length;
	op_t *operations;
} op_set_t;

unsigned long
getval(a, regs)
	atom_t a;
	unsigned long (*regs)[reg_kinds];
{
	if (a.type == atom_register)
		return (*regs)[a.value];

	return a.value;
}

/*
 * [0] = equal
 * [1] = less
 * [2] = greater
 */
static unsigned short
jmp_states[][3] = {
	[op_ja - op_ja] = {
		[0] = 1,
		[1] = 1,
		[2] = 1,
	},
	[op_jz - op_ja] = {
		[0] = 1,
		[1] = 0,
		[2] = 0,
	},
	[op_jl - op_ja] = {
		[0] = 0,
		[1] = 1,
		[2] = 0,
	},
	[op_jg - op_ja] = {
		[0] = 0,
		[1] = 0,
		[2] = 1,
	},
	[op_jle - op_ja] = {
		[0] = 1,
		[1] = 1,
		[2] = 0,
	},
	[op_jge - op_ja] = {
		[0] = 1,
		[1] = 0,
		[2] = 1,
	},
	[op_jnz - op_ja] = {
		[0] = 0,
		[1] = 1,
		[2] = 1,
	},
};

void
eval(ops, regs)
	op_set_t ops;
	unsigned long (*regs)[reg_kinds];
{
	while (ops.index < ops.length) {
		op_t op = ops.operations[ops.index];
		
		switch (op.type) {
			case op_mv:
				if (op.argv[0].type != atom_register) break;
				(*regs)[op.argv[0].value] = getval(op.argv[1], regs);
				break;

			case op_add:
				if (op.argv[0].type != atom_register) break;
				(*regs)[op.argv[0].value] += getval(op.argv[1], regs);
				break;

			case op_sub:
				if (op.argv[0].type != atom_register) break;
				(*regs)[op.argv[0].value] -= getval(op.argv[1], regs);
				break;

			case op_mul:
				if (op.argv[0].type != atom_register) break;
				(*regs)[op.argv[0].value] *= getval(op.argv[1], regs);
				break;

			case op_div:
				if (op.argv[0].type != atom_register) break;
				(*regs)[op.argv[0].value] /= getval(op.argv[1], regs);
				break;

			case op_mod:
				if (op.argv[0].type != atom_register) break;
				(*regs)[op.argv[0].value] %= getval(op.argv[1], regs);
				break;

			case op_cmp: {
				unsigned short val = 0;
				unsigned long v1 = getval(op.argv[0], regs);
				unsigned long v2 = getval(op.argv[1], regs);

				if (v1 < v2)
					val = 1;
					
				if (v1 > v2)
					val = 2;
				
				(*regs)[reg_g] = val;
				break;
			}

			case op_ja:
			case op_jz:
			case op_jnz:
			case op_jl:
			case op_jg:
			case op_jle:
			case op_jge:
				if (jmp_states[op.type - op_ja][(*regs)[reg_g]]) {
					ops.index = getval(op.argv[0], regs);
					continue;
				}
				break;

			case op_prn:
				printf("%ld\n", getval(op.argv[0], regs));
				break;

			default:
				break;
		}
		
		ops.index += 1;
	}
}

void
free_op(op)
	op_t op;
{
	free(op.argv);
}

void
free_op_set(ops)
	op_set_t ops;
{
	for (unsigned long i = 0; i < ops.length; ++i)
		free_op(ops.operations[i]);

	free(ops.operations);
}

atom_t
reg(r)
	reg_t r;
{
	return (atom_t)
	{
		.type = atom_register,
		.value = r,
	};
}

atom_t
num(n)
	unsigned long n;
{
	return (atom_t)
	{
		.type = atom_number,
		.value = n,
	};
}

/*
void
print_op(o)
	op_t o;
{
	printf("op id: %d\n", o.type);
	printf("arg1v: %ld\n", o.argv[0].value);
	printf("arg1t: %d\n", o.argv[0].type);
	if (o.argc == 2) {
		printf("arg2v: %ld\n", o.argv[1].value);
		printf("arg2t: %d\n", o.argv[1].type);
	}
}

void
print_op_set(s)
	op_set_t s;
{
	for (unsigned long i = 0; i < s.length; ++i) {
		print_op(s.operations[i]);
	}
}
*/
#endif /* !SBM_H */
