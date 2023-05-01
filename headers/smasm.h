#ifndef SMASM_H
#	define SMASM_H

#	include "sm.h"
#	include "strmap.h"

atom_kind_t
get_atom_type(str)
	char *str;
{
	if (str[1] != '\0')
		return atom_number;

	switch (str[0]) {
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
			return atom_register;
		default:
			return atom_number;
	}
}


reg_t
get_reg(str)
	char *str;
{
	return *str - 'a';
}

unsigned long
get_num(str)
	char *str;
{
	return atoi(str);
}

atom_t
get_atom(str)
	char *str;
{	
	if (get_atom_type(str) == atom_register)
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
		{"mv", op_mv},
		{"add", op_add},
		{"sub", op_sub},
		{"mul", op_mul},
		{"div", op_div},
		{"mod", op_mod},
		{"cmp", op_cmp},
		{"ja", op_ja},
		{"jz", op_jz},
		{"jl", op_jl},
		{"jg", op_jg},
		{"jnz", op_jnz},
		{"jle", op_jle},
		{"jge", op_jge},
		{"prn", op_prn},
		{"jb", op_jb},
		{"gb", op_gb},
		{"swp", op_swp},
		{"shl", op_shl},
		{"shr", op_shr},
		{"and", op_and},
		{"xor", op_xor},
		{"or", op_or},
		{"not", op_not},
		{"inc", op_inc},
		{"dec", op_dec},
		{"hlt", op_hlt},
	};
	
	static const size_t op_count = sizeof(op_table) / sizeof(op_table[0]);
	unsigned long i;

	for (i = 0; i < op_count; ++i)
		if (!strcmp(str, op_table[i].name))
			return op_table[i].kind;

	return op_null;
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
	unsigned long current_line = 0;

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
	
	if (strl_map_index(labels, "_start"))
		ops.index = strl_map_get(labels,"_start");

	char *str = strdup(s);
	char *str_p = str;
	char *l;
	char *token;
	char *line;
	char *line_p;
	unsigned short line_start;
	unsigned long current_line = 0;

	while ((l = strtok_r(str, "\n", &str)) != NULL) {
		line_start = 1;
		line = strdup(l);
		line_p = line;

		if (line[0] != '\0' && line[strlen(line)-1] == ':') {
			// labels already parsed, ignoring
		} else {
			while ((token = strtok_r(line, " ", &line)) != NULL) {
				if (line_start) {
					++ops.length;
					ops.operations = realloc(ops.operations, sizeof(op_t) * ops.length);
					ops.operations[ops.length - 1].type = get_op_type(token);
					ops.operations[ops.length - 1].argc = 0;
					ops.operations[ops.length - 1].argv = malloc(sizeof(atom_t) * 1);
					line_start = 0;
				} else {
					++ops.operations[ops.length - 1].argc;
					ops.operations[ops.length - 1].argv = realloc(ops.operations[ops.length - 1].argv, sizeof(atom_t) * ops.operations[ops.length - 1].argc);
					char *arg = strdup(token);

					if (strl_map_index(labels, arg) > 0) {
						free(arg);
						arg = malloc(sizeof(char) * 20);
						snprintf(arg, 20, "%lu", strl_map_get(labels, token));
					}
					ops.operations[ops.length - 1].argv[ops.operations[ops.length - 1].argc - 1] = get_atom(arg);
					free(arg);
				}
			}
		}

		free(line_p);
		++current_line;
	}

	free(str_p);
	strl_map_free(labels);

	return ops;
}

#endif /* !SMASM_H */
