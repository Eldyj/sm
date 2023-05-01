#ifndef STRMAP_H_
#	define STRMAP_H_

#	include <stdlib.h>
#	include <string.h>

typedef struct {
	char **indexes;
	unsigned long *values;
	unsigned long length;
} strl_map_t;

strl_map_t
new_strl_map()
{
	strl_map_t result = {
		.indexes = malloc(sizeof(char *) * 0),
		.values = malloc(sizeof(unsigned long) * 0),
		.length = 0,
	};

	return result;
}

void
strl_map_append(map, i, v)
	strl_map_t *map;
	char *i;
	unsigned long v;
{
	++map->length;
	map->indexes = realloc(map->indexes, sizeof(char *) * map->length);
	map->values = realloc(map->values, sizeof(unsigned long) * map->length);
	map->indexes[map->length-1] = strdup(i);
	map->values[map->length-1] = v; 
}

unsigned long
strl_map_index(map, i)
	strl_map_t map;
	char *i;
{
	for (unsigned long j = 0; j < map.length; ++j)
		if (!strcmp(map.indexes[j], i))
			return j+1;

	return 0;
}

unsigned long
strl_map_get(map, i)
	strl_map_t map;
	char *i;
{
	unsigned long index = strl_map_index(map, i);
	
	if (!index)
		exit(1);

	return map.values[index-1];
}

void
strl_map_free(map)
	strl_map_t map;
{
	free(map.values);
	
	while (map.length)
		free(map.indexes[map.length---1]);
		
	free(map.indexes);
}

#endif /* !STRMAP_H_ */
