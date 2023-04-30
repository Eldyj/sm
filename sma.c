#include "headers/smbin.h"
#include "headers/smasm.h"

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


int
main(argc, argv)
	int argc;
	char *argv[argc];
{
	if (argc < 3)
		return 1;
	
	static unsigned long regs = {0};
	
	if (!strcmp(argv[1], "run")) {
		char *fc = read_file(argv[2]);
		op_set_t ops = from_asm(fc);
		free(fc);
		eval(ops, &regs);
		free_op_set(ops);
	} else if (!strcmp(argv[1], "compile")) {
		if (argc < 4)
			return 1;

		char *fc = read_file(argv[2]);
		op_set_t ops = from_asm(fc);
		free(fc);
		//print_op_set(ops);
		to_bytes(argv[3], ops);
		free_op_set(ops);
	} else {
		return 1;
	}
}
