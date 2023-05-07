#ifndef SMBIN_C
#	define SMBIN_C

#	include "smbin.h"
#	include <stdio.h>
#	include <stdlib.h>

void
to_bytes(fn, ops)
	char *fn;
	op_set_t ops;
{
	op_t op;
	uint8_t unit8;
	sm_unit_t unit_d;
	atom_t atom;

	FILE* fp = fopen(fn, "wb");
	if (!fp) {
		perror("Error opening file");
		exit(1);
	}

	unit_d = ops.index;
	fwrite(&unit_d, sizeof(unit_d), 1, fp);

	for (size_t i = 0; i < ops.length; ++i) {
		op = ops.operations[i];
		unit8 = op.type;
		fwrite(&unit8, sizeof(unit8), 1, fp);
		unit8 = op.argc;
		fwrite(&unit8, sizeof(unit8), 1, fp);

		for (size_t j = 0; j < op.argc; ++j) {
			atom = op.argv[j];
			unit8 = atom.type;
			fwrite(&unit8, sizeof(unit8), 1, fp);

			unit_d = atom.value;
			fwrite(&unit_d, sizeof(unit_d), 1, fp);		  
		}
	}

	fclose(fp);
}

op_set_t
from_bytes(fn)
	char *fn;
{
	op_set_t ops = {
		.index = 0,
		.length = 0,
		.operations = malloc(sizeof(op_t)*0),
	};

	FILE* fp = fopen(fn, "rb");
	if (!fp) {
		perror("Error opening file");
		exit(1);
	}

	op_t op;
	uint8_t unit8;
	sm_unit_t unit_d;
	atom_t atom;

	fread(&unit_d, sizeof(unit_d), 1, fp);
	ops.index = unit_d;

	while (fread(&unit8, sizeof(unit8), 1, fp) == 1) {
		op.type = unit8;
		fread(&unit8, sizeof(unit8), 1, fp);
		op.argc = unit8;

		op.argv = malloc(sizeof(atom_t) * op.argc);

		for (size_t j = 0; j < op.argc; ++j) {
			fread(&unit8, sizeof(unit8), 1, fp);
			atom.type = unit8;
			fread(&unit_d, sizeof(unit_d), 1, fp);
			atom.value = unit_d;
			op.argv[j] = atom;
		}

		++ops.length;
		ops.operations = realloc(ops.operations, sizeof(op_t)*ops.length);
		ops.operations[ops.length - 1] = op;
	}

	fclose(fp);

	return ops;
}

#endif /* !SMBIN_C */
