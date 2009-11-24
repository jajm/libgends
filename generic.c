#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generic.h"

generic_t generic(u8 size, void *data)
{
	generic_t g;

	if(size == 0 || data == NULL)
		return NULL;

	g = malloc(sizeof(generic_t));
	if(g == NULL)
		return NULL;
	g->d = malloc(size);
	if(g->d == NULL){
		free(g);
		return NULL;
	}
	g->sz = size;
	memmove(g->d, data, g->sz);

	return g;
}

err_code generic_affect(generic_t *g, u8 size, void *data)
{
	if(size == 0 || data == NULL)
		return PARAM_VALUE_ERROR;
	if(*g == NULL){
		*g = generic(size, data);
	}else if((*g)->sz == size){
		/* Pas de réallocation à faire */
		memmove((*g)->d, data, (*g)->sz);
	}else{
		(*g)->d = realloc((*g)->d, size);
		if((*g)->d == NULL)
			return MEM_ALLOC_ERROR;
		(*g)->sz = size;
		memmove((*g)->d, data, (*g)->sz);
	}
	return OK;
}

err_code generic_copy(generic_t *to, const generic_t from)
{
	return generic_affect(to, from->sz, from->d);
}

u8 generic_size(const generic_t g)
{
	if(g == NULL)
		return -1;
	return g->sz;
}

void *generic_data(const generic_t g)
{
	if(g == NULL)
		return NULL;
	return g->d;
}

err_code generic_free(generic_t *g)
{
	if(*g != NULL){
		free((*g)->d);
		free(*g);
		*g = NULL;
	}

	return OK;
}

