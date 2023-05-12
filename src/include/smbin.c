#ifndef SMBIN_C
#	define SMBIN_C

#	include <smbin.h>
#	include <stdio.h>
#	include <stdlib.h>

void
to_bytes(fn, ops)
	char *fn;
	op_set_t ops;
{
	op_t op;
	uint8_t unit8 = 0;
	sm_unit_t unit_d = 0;
	atom_t atom;

	FILE* fp = fopen(fn, "wb");
	if (!fp) {
		perror("Error opening file");
		exit(1);
	}

	unit_d = ops.index;
	fwrite(&unit_d, sizeof(unit_d), 1, fp);
	unit_d = ops.length;
	fwrite(&unit_d, sizeof(unit_d), 1, fp);

	for (size_t i = 0; i < ops.length; ++i) {
		op = ops.operations[i];
		unit8 = op.type;
		fwrite(&unit8, sizeof(unit8), 1, fp);

		for (size_t j = 0; j < op_argc[op.type]; ++j) {
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
	op_set_t ops;

	FILE* fp = fopen(fn, "rb");
	if (!fp) {
		perror("Error opening file");
		exit(1);
	}

	op_t op;
	uint8_t unit8 = 0;
	sm_unit_t unit_d = 0;
	atom_t atom;
	size_t l = 0;

	fread(&unit_d, sizeof(unit_d), 1, fp);
	ops.index = unit_d;
	fread(&unit_d, sizeof(unit_d), 1, fp);
	ops.length = unit_d;
	ops.operations = malloc(sizeof(op_t) * (unit_d+1));

	while (fread(&unit8, sizeof(unit8), 1, fp) == 1) {
		op.type = unit8;
		uint8_t argc = op_argc[op.type];
		op.argv = malloc(sizeof(atom_t) * argc);

		for (size_t j = 0; j < argc; ++j) {
			fread(&unit8, sizeof(unit8), 1, fp);
			atom.type = unit8;
			fread(&unit_d, sizeof(unit_d), 1, fp);
			atom.value = unit_d;
			op.argv[j] = atom;
		}

		ops.operations[l++] = op;
	}

	fclose(fp);
	return ops;
}

#endif /* !SMBIN_C */
