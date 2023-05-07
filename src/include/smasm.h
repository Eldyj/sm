#ifndef SMASM_H
#	define SMASM_H

#	include <sm.h>
#	include <strmap.h>

#	ifndef SMASM_C
extern atom_kind_t get_atom_type(char *str);
extern reg_t get_reg(char *str);
extern sm_unit_t get_num(char *str);
extern atom_t get_atom(char *str);
extern op_kind_t get_op_type(char *str);
extern strl_map_t parse_labels(char *s);
extern op_set_t from_asm(char *s);
# endif /* !SMASM_C */

#endif /* !SMASM_H */
