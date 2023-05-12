#include <smbin.h>
#include <smasm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char
*read_file(fn)
	char *fn;
{
	FILE *f = fopen(fn, "r");
	
	if (!f)
		return NULL;

	fseek(f, 0, SEEK_END);
	long fs = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* buffer = malloc(sizeof(char)*(fs+1));
	if (!buffer) {
		fclose(f);
		return NULL;
	}

	fread(buffer, fs, 1, f);
	fclose(f);

	buffer[fs] = '\0';
	return buffer;
}

char
char_uppercase(c)
	char c;
{
	if (c <= 'z' && c >= 'a')
		return c + ('A'- 'a');
	return c;
}

char
*str_uppercase(s)
	char *s;
{
	size_t sl = strlen(s);

	for (size_t i = 0; i < sl; ++i)
		s[i] = char_uppercase(s[i]);

	return s;
}

int
main(argc, argv)
	int argc;
	char *argv[argc];
{
	if (argc < 3)
		return 1;
	
	static
	sm_unit_t regs[REG_KINDS] = {0};
	
	if (!strcmp(argv[1], "run")) {
		char *fc = str_uppercase(read_file(argv[2]));
		op_set_t ops = from_asm(fc);
		free(fc);
		eval(ops, &regs);
		free_op_set(ops);
	} else if (!strcmp(argv[1], "compile")) {
		if (argc < 4)
			return 1;

		char *fc = str_uppercase(read_file(argv[2]));
		op_set_t ops = from_asm(fc);
		free(fc);
		to_bytes(argv[3], ops);
		free_op_set(ops);
	} else {
		return 1;
	}
}
