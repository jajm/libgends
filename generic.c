#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generic.h"

err_code generic_init(generic_t *g)
{
	if(g == NULL)
		return PARAM_VALUE_ERROR;
	g->d = NULL;
	g->sz = 0;

	return OK;
}

err_code generic_affect(generic_t *g, u8 size, void *data)
{
	if(g == NULL || size == 0 || data == NULL)
		return PARAM_VALUE_ERROR;
	if(g->sz == size){
		/* Pas de réallocation à faire */
		memmove(g->d, data, g->sz);
	}else{
		g->d = realloc(g->d, size);
		if(g->d == NULL)
			return MEM_ALLOC_ERROR;
		g->sz = size;
		memmove(g->d, data, g->sz);
	}
	return OK;
}

err_code generic_copy(generic_t *to, const generic_t *from)
{
	return generic_affect(to, from->sz, from->d);
}

u8 generic_size(const generic_t *g)
{
	if(g == NULL)
		return -1;
	return g->sz;
}

void *generic_data(const generic_t *g)
{
	if(g == NULL)
		return NULL;
	return g->d;
}

err_code generic_delete(generic_t *g)
{
	if(g == NULL)
		return PARAM_VALUE_ERROR;
	free(g->d);
	g->d = NULL;
	g->sz = 0;

	return OK;
}

