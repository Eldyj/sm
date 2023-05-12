#ifndef SMASM_C
#	define SMASM_C

# include <smasm.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <string.h>

atom_kind_t
get_atom_type(str)
	char *str;
{
	if (str[1] != '\0')
		return ATOM_NUM;

	switch (str[0]) {
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
			return ATOM_REG;
		default:
			return ATOM_NUM;
	}
}


reg_t
get_reg(str)
	char *str;
{
	return *str - 'A';
}

sm_unit_t
get_num(str)
	char *str;
{
	return atoi(str);
}

atom_t
get_atom(str)
	char *str;
{	
	if (get_atom_type(str) == ATOM_REG)
		return reg(get_reg(str));

	return num(get_num(str));
}

op_kind_t
get_op_type(str)
	char *str;
{
	static const struct {
		const char *name;
		op_kind_t kind;
	} op_table[] = {
		{"MV", OP_MV},
		{"ADD", OP_ADD},
		{"SUB", OP_SUB},
		{"MUL", OP_MUL},
		{"DIV", OP_DIV},
		{"MOD", OP_MOD},
		{"CMP", OP_CMP},
		{"JA", OP_JA},
		{"JZ", OP_JZ},
		{"JL", OP_JL},
		{"JG", OP_JG},
		{"JNZ", OP_JNZ},
		{"JLE", OP_JLE},
		{"JGE", OP_JGE},
		{"PRN", OP_PRN},
		{"JB", OP_JB},
		{"GB", OP_GB},
		{"SWP", OP_SWP},
		{"SHL", OP_SHL},
		{"SHR", OP_SHR},
		{"AND", OP_AND},
		{"XOR", OP_XOR},
		{"OR", OP_OR},
		{"NOT", OP_NOT},
		{"INC", OP_INC},
		{"DEC", OP_DEC},
		{"HLT", OP_HLT},
		{"PSH", OP_PSH},
		{"POP", OP_POP},
	};
	
	static const size_t op_count = sizeof(op_table) / sizeof(op_table[0]);

	for (size_t i = 0; i < op_count; ++i)
		if (!strcmp(str, op_table[i].name))
			return op_table[i].kind;

	fprintf(stderr, "unknown instruction: %s\n", str);
	exit(1);

	return OP_NIL;
}


strl_map_t
parse_labels(s)
	char *s;
{
	strl_map_t labels = new_strl_map();
	char *str = strdup(s);
	char *str_p = str;
	char *l;
	char *line;
	size_t current_line = 0;

	while ((l = strtok_r(str, "\n", &str)) != NULL) {
		line = strdup(l);
		if (line[0] != '\0' && line[strlen(line)-1] == ':') {
			line[strlen(line)-1] = '\0';
			strl_map_append(&labels, line, current_line--);
		}
		free(line);
		++current_line;
	}

	free(str_p);
	return labels;
}


op_set_t
from_asm(s)
	char *s;
{
	op_set_t ops = {
		.index = 0,
		.length = 0,
		.operations = malloc(sizeof(op_t) * 0),
	};

	strl_map_t labels = parse_labels(s);
	
	if (strl_map_index(labels, "_START"))
		ops.index = strl_map_get(labels,"_START");

	char *str = strdup(s);
	char *str_p = str;
	char *l;
	char *token;
	char *line;
	char *line_p;
	uint8_t line_start;
	uint8_t j = 0;
	op_kind_t ok = OP_NIL;

	while ((l = strtok_r(str, "\n", &str)) != NULL) {
		line_start = 1;
		line = strdup(l);
		line_p = line;

		if (line[0] != '\0' && line[strlen(line)-1] == ':') {
			// labels already parsed, ignoring
		} else {
			while ((token = strtok_r(line, " ", &line)) != NULL) {
				if (line_start) {
					if (j != op_argc[ok]) {
						fprintf(stderr, "wrong arguments count, given: %d, expected %d\n", j, op_argc[ok]);
						exit(1);				
					}
					
					++ops.length;
					ops.operations = realloc(ops.operations, sizeof(op_t) * ops.length);
					ops.operations[ops.length - 1].type = ok = get_op_type(token);
					ops.operations[ops.length - 1].argv = malloc(sizeof(atom_t) * op_argc[ok]);
					line_start = 0;
					j = 0;
				} else {
					char *arg = strdup(token);

					if (strl_map_index(labels, arg) > 0) {
						free(arg);
						arg = malloc(sizeof(char) * 20);
						snprintf(arg, 20, "%lu", strl_map_get(labels, token));
					}
					
					ops.operations[ops.length - 1].argv[j++] = get_atom(arg);
					free(arg);
				}
			}
		}
		
		if (j != op_argc[ok]) {
			fprintf(stderr, "wrong arguments count, given: %d, expected %d\n", j, op_argc[ok]);
			exit(1);				
		}

		free(line_p);
	}

	free(str_p);
	strl_map_free(labels);

	return ops;
}

#endif /* !SMASM_C */
