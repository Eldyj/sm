#ifndef SMBIN_H
#	define SMBIN_H

#	include <sm.h>

#	ifndef SMBIN_C
extern void to_bytes(char *fn, op_set_t ops);
extern op_set_t from_bytes(char *fn);
#	endif /* !SMBIN_C */

#endif /* !SMBIN_H */
