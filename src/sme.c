#include <smbin.h>

int
main(argc, argv)
	int argc;
	char *argv[argc];
{
	if (argc < 2)
		return 1;

	static sm_unit_t regs[reg_kinds] = {0};
	op_set_t ops = from_bytes(argv[1]);
	eval(ops, &regs);
	free_op_set(ops);
	return 0;
}
