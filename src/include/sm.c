#ifndef SM_C
#	define SM_C

#	include <sm.h>
#	include <stdio.h>
#	include <stdlib.h>

sm_unit_t
getval(a, regs)
	atom_t a;
	sm_unit_t (*regs)[reg_kinds];
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
static
uint8_t
jmp_states[][3] = {
	[op_jz - op_jz] = {
		[0] = 1,
		[1] = 0,
		[2] = 0,
	},
	[op_jl - op_jz] = {
		[0] = 0,
		[1] = 1,
		[2] = 0,
	},
	[op_jg - op_jz] = {
		[0] = 0,
		[1] = 0,
		[2] = 1,
	},
	[op_jle - op_jz] = {
		[0] = 1,
		[1] = 1,
		[2] = 0,
	},
	[op_jge - op_jz] = {
		[0] = 1,
		[1] = 0,
		[2] = 1,
	},
	[op_jnz - op_jz] = {
		[0] = 0,
		[1] = 1,
		[2] = 1,
	},
};

void
eval(ops, regs)
	op_set_t ops;
	sm_unit_t (*regs)[reg_kinds];
{
	static
	size_t jb_stack[256] = {0};
	uint8_t jb_index = 0;
	
	static
	sm_unit_t sm_stack[8192] = {0};
	size_t sm_sindex = 0;
	
	while (ops.index < ops.length) {
		op_t op = ops.operations[ops.index];
		
		switch (op.type) {
			case op_psh: {
				sm_stack[sm_sindex++] = getval(op.argv[0], regs);
				break;
			}

			case op_pop: {
				if (op.argv[0].type != atom_register)
					break;

				--sm_sindex;
				(*regs)[op.argv[0].value] = sm_stack[sm_sindex];
				break;
			}
		
			case op_mv: {
				if (op.argv[0].type != atom_register)
					break;
				
				(*regs)[op.argv[0].value] = getval(op.argv[1], regs);
				break;
			}

			case op_swp: {
				if (op.argv[0].type != atom_register)
					break;
					
				if (op.argv[1].type != atom_register)
					break;

				uint8_t v = (*regs)[op.argv[0].value];
				(*regs)[op.argv[0].value] = getval(op.argv[1], regs);
				(*regs)[op.argv[1].value] = v;
				break;
			}

			case op_add: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] += getval(op.argv[1], regs);
				break;
			}

			case op_sub: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] -= getval(op.argv[1], regs);
				break;
			}

			case op_mul: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] *= getval(op.argv[1], regs);
				break;
			}

			case op_div: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] /= getval(op.argv[1], regs);
				break;
			}

			case op_mod: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] %= getval(op.argv[1], regs);
				break;
			}

			case op_shl: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] <<= getval(op.argv[1], regs);
				break;
			}

			case op_shr: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] >>= getval(op.argv[1], regs);
				break;
			}

			case op_and: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] &= getval(op.argv[1], regs);
				break;
			}

			case op_xor: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] ^= getval(op.argv[1], regs);
				break;
			}

			case op_or: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] |= getval(op.argv[1], regs);
				break;
			}
				
			case op_not: {
				if (op.argv[0].type != atom_register)
					break;
					
				(*regs)[op.argv[0].value] = ~(*regs)[op.argv[0].value];
				break;
			}

			case op_inc: {
				if (op.argv[0].type != atom_register)
					break;
					
				++(*regs)[op.argv[0].value];
				break;
			}

			case op_dec: {
				if (op.argv[0].type != atom_register)
					break;
					
				--(*regs)[op.argv[0].value];
				break;
			}

			case op_cmp: {
				uint8_t val = 0;
				sm_unit_t v1 = getval(op.argv[0], regs);
				sm_unit_t v2 = getval(op.argv[1], regs);

				if (v1 < v2)
					val = 1;
					
				if (v1 > v2)
					val = 2;
				
				(*regs)[reg_g] = val;
				break;
			}

			case op_ja: {
				jmp:
				ops.index = getval(op.argv[0], regs);
				continue;
				break;
			}
			
			case op_jz:
			case op_jnz:
			case op_jl:
			case op_jg:
			case op_jle:
			case op_jge: {
				if (jmp_states[op.type - op_jz][(*regs)[reg_g]])
					goto jmp;
					
				break;
			}

			case op_jb: {
				jb_stack[jb_index++] = ops.index+1;
				goto jmp;
				break;
			}

			case op_gb: {
				ops.index = jb_stack[jb_index---1];
				continue;
				break;
			}

			case op_prn: {
				printf("%u\n", getval(op.argv[0], regs));
				break;
			}

			case op_hlt: {
				exit(getval(op.argv[0], regs));
				break;
			}

			default: {
				exit(1);
				break;
			}
		}
		
		++ops.index;
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
	while (ops.length)
		free_op(ops.operations[ops.length---1]);

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
	sm_unit_t n;
{
	return (atom_t)
	{
		.type = atom_number,
		.value = n,
	};
}

#endif /* !SM_C */
