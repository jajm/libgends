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

u8 generic_cmp(generic_t g1, generic_t g2)
{
	/* S'ils sont tous les 2 à NULL, on dira qu'ils sont égaux */
	if(g1 == NULL && g2 == NULL){
		return 0;
	/* Si l'un des deux est à NULL, c'est l'autre le plus grand */
	}else if(g1 == NULL && g2 != NULL){
		return -1;
	}else if(g1 != NULL && g2 == NULL){
		return 1;
	/* Si aucun des 2 n'est à NULL, on compare d'abord la taille,
	 * et ensuite la valeur des données */
	}else if(g1 != NULL && g2 != NULL){
		if(g1->sz == g2->sz){
			return memcmp(g1->d, g2->d, g1->sz);
		}else if(g1->sz < g2->sz){
			if(memcmp(g1->d, g2->d, g1->sz) <= 0)
				return -1;
			else
				return 1;
		}else if(g1->sz > g2->sz){
			if(memcmp(g1->d, g2->d, g2->sz) >= 0)
				return 1;
			else
				return -1;
		}
	}
	return 0;
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

