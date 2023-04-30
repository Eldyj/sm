#include "headers/smbin.h"
#include <string.h>

int
main(argc, argv)
	int argc;
	char *argv[argc];
{
	if (argc < 2)
		return 1;

	static unsigned long regs[reg_kinds] = {0};
	op_set_t ops = from_bytes(argv[1]);
	//print_op_set(ops);
	eval(ops, &regs);
	free_op_set(ops);
	return 0;
}
