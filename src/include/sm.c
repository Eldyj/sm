#ifndef SM_C
#	define SM_C

#	include <sm.h>
#	include <stdio.h>
#	include <stdlib.h>

sm_unit_t
getval(a, regs)
	atom_t a;
	sm_unit_t (*regs)[REG_KINDS];
{
	if (a.type == ATOM_REG)
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
	[OP_JZ - OP_JZ] = {
		[0] = 1,
		[1] = 0,
		[2] = 0,
	},
	[OP_JL - OP_JZ] = {
		[0] = 0,
		[1] = 1,
		[2] = 0,
	},
	[OP_JG - OP_JZ] = {
		[0] = 0,
		[1] = 0,
		[2] = 1,
	},
	[OP_JLE - OP_JZ] = {
		[0] = 1,
		[1] = 1,
		[2] = 0,
	},
	[OP_JGE - OP_JZ] = {
		[0] = 1,
		[1] = 0,
		[2] = 1,
	},
	[OP_JNZ - OP_JZ] = {
		[0] = 0,
		[1] = 1,
		[2] = 1,
	},
};

void
eval(ops, regs)
	op_set_t ops;
	sm_unit_t (*regs)[REG_KINDS];
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
			case OP_PSH: {
				sm_stack[sm_sindex++] = getval(op.argv[0], regs);
				break;
			}

			case OP_POP: {
				if (op.argv[0].type != ATOM_REG)
					break;

				--sm_sindex;
				(*regs)[op.argv[0].value] = sm_stack[sm_sindex];
				break;
			}
		
			case OP_MV: {
				if (op.argv[0].type != ATOM_REG)
					break;
				
				(*regs)[op.argv[0].value] = getval(op.argv[1], regs);
				break;
			}

			case OP_SWP: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				if (op.argv[1].type != ATOM_REG)
					break;

				uint8_t v = (*regs)[op.argv[0].value];
				(*regs)[op.argv[0].value] = getval(op.argv[1], regs);
				(*regs)[op.argv[1].value] = v;
				break;
			}

			case OP_ADD: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] += getval(op.argv[1], regs);
				break;
			}

			case OP_SUB: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] -= getval(op.argv[1], regs);
				break;
			}

			case OP_MUL: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] *= getval(op.argv[1], regs);
				break;
			}

			case OP_DIV: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] /= getval(op.argv[1], regs);
				break;
			}

			case OP_MOD: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] %= getval(op.argv[1], regs);
				break;
			}

			case OP_SHL: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] <<= getval(op.argv[1], regs);
				break;
			}

			case OP_SHR: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] >>= getval(op.argv[1], regs);
				break;
			}

			case OP_AND: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] &= getval(op.argv[1], regs);
				break;
			}

			case OP_XOR: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] ^= getval(op.argv[1], regs);
				break;
			}

			case OP_OR: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] |= getval(op.argv[1], regs);
				break;
			}
				
			case OP_NOT: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				(*regs)[op.argv[0].value] = ~(*regs)[op.argv[0].value];
				break;
			}

			case OP_INC: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				++(*regs)[op.argv[0].value];
				break;
			}

			case OP_DEC: {
				if (op.argv[0].type != ATOM_REG)
					break;
					
				--(*regs)[op.argv[0].value];
				break;
			}

			case OP_CMP: {
				uint8_t val = 0;
				sm_unit_t v1 = getval(op.argv[0], regs);
				sm_unit_t v2 = getval(op.argv[1], regs);

				if (v1 < v2)
					val = 1;
					
				if (v1 > v2)
					val = 2;
				
				(*regs)[REG_G] = val;
				break;
			}

			case OP_JA: {
				jmp:
				ops.index = getval(op.argv[0], regs);
				continue;
				break;
			}
			
			case OP_JZ:
			case OP_JNZ:
			case OP_JL:
			case OP_JG:
			case OP_JLE:
			case OP_JGE: {
				if (jmp_states[op.type - OP_JZ][(*regs)[REG_G]])
					goto jmp;
					
				break;
			}

			case OP_JB: {
				jb_stack[jb_index++] = ops.index+1;
				goto jmp;
				break;
			}

			case OP_GB: {
				ops.index = jb_stack[jb_index---1];
				continue;
				break;
			}

			case OP_PRN: {
				printf("%u\n", getval(op.argv[0], regs));
				break;
			}

			case OP_HLT: {
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
		.type = ATOM_REG,
		.value = r,
	};
}

atom_t
num(n)
	sm_unit_t n;
{
	return (atom_t)
	{
		.type = ATOM_NUM,
		.value = n,
	};
}

#endif /* !SM_C */
