#ifndef STRMAP_H_
#	define STRMAP_H_

#	include <stddef.h>

typedef struct {
	char **indexes;
	unsigned long *values;
	size_t length;
} strl_map_t;

#	ifndef STRMAP_C_
extern strl_map_t new_strl_map(void);
extern void strl_map_append(strl_map_t *map, char *i, unsigned long v);
extern size_t strl_map_index(strl_map_t map, char *i);
extern unsigned long strl_map_get(strl_map_t map, char *i);
extern void strl_map_free(strl_map_t map);
#	endif /* !STRMAP_C_ */

#endif /* !STRMAP_H_ */
