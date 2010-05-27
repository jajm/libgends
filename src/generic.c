#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/types.h"
#include "core/error.h"
#include "generic.h"

generic_t * generic(const char *type_name, const void *data_ptr)
{
	generic_t *g;
	u32 data_size;
	size_t length;

	if(type_name == NULL || data_ptr == NULL){
		Error("Bad parameter value");
		return NULL;
	}

	data_size = type_sizeof(type_name);
	if(data_size == 0){
		ErrorP("Failed to retrieve '%s' size", type_name);
		return NULL;
	}
	length = strlen(type_name);

	g = malloc(sizeof(generic_t));
	if(g == NULL){
		Error("Memory allocation error");
		return NULL;
	}
	g->data_ptr = malloc(data_size);
	if(g->data_ptr == NULL){
		free(g);
		Error("Memory allocation error");
		return NULL;
	}
	g->type_name = malloc(length+1);
	if(g->type_name == NULL){
		free(g->data_ptr);
		free(g);
		Error("Memory allocation error");
		return NULL;
	}
	memmove(g->data_ptr, data_ptr, data_size);
	strncpy(g->type_name, type_name, length+1);
	
	return g;
}

s8 generic_affect(generic_t **g, const char *type_name, const void *data_ptr)
{
	u32 data_size;
	size_t length;

	if(g == NULL || type_name == NULL || data_ptr == NULL){
		Error("Bad parameters");
		return -1;
	}

	data_size = type_sizeof(type_name);
	if(data_size == 0){
		ErrorP("Failed to retrieve '%s' size", type_name);
		return -1;
	}
	length = strlen(type_name);
	
	if(*g == NULL){
		*g = generic(type_name, data_ptr);
	}else if(strcmp((*g)->type_name, type_name) == 0){
		/* Pas de réallocation à faire */
		memmove((*g)->data_ptr, data_ptr, data_size);
	}else{
		(*g)->type_name = realloc((*g)->type_name, length);
		strncpy((*g)->type_name, type_name, length);
		if(type_sizeof((*g)->type_name) != data_size){
			(*g)->data_ptr = realloc((*g)->data_ptr, data_size);
			if((*g)->data_ptr == NULL){
				Error("Memory allocation error");
				return -1;
			}
		}
		memmove((*g)->data_ptr, data_ptr, data_size);
	}

	return 0;
}

s8 generic_copy(generic_t **to, const generic_t *from)
{
	return generic_affect(to, from->type_name, from->data_ptr);
}

s32 generic_cmp(const generic_t *g1, const generic_t *g2)
{
	func_ptr_t cmp_f;
	u32 size1, size2;
	s32 cmp;
	void *ptr;

	/* S'ils sont tous les 2 à NULL, on dira qu'ils sont égaux */
	if(g1 == NULL && g2 == NULL){
		cmp = 0;
	/* Si l'un des deux est à NULL, c'est l'autre le plus grand */
	}else if(g1 == NULL && g2 != NULL){
		cmp = -1;
	}else if(g1 != NULL && g2 == NULL){
		cmp = 1;
	/* Si aucun des deux n'est à NULL, qu'ils sont du même type,
	 * et qu'ils possèdent une fonction de comparaison, on utilise
	 * cette dernière */
	}else if((cmp_f = type_get_func(g1->type_name, "cmp")) != NULL
	  && strcmp(g1->type_name, g2->type_name) == 0){
		cmp = cmp_f(g1->data_ptr, g2->data_ptr);
	/* Sinon, on compare d'abord la taille,
	 * et ensuite la valeur des données */
	}else{
		size1 = type_sizeof(g1->type_name);
		size2 = type_sizeof(g2->type_name);
		if(size1 == size2){
			cmp = memcmp(g1->data_ptr, g2->data_ptr, size1);
		}else{
			u32 diff = (size1<size2)?(size2-size1):(size1-size2);
			u32 min = size1<size2 ? size1 : size2;
			cmp = memcmp(g1->data_ptr, g2->data_ptr, min);
			ptr = calloc(diff, 1);
			if(ptr){
				if(size1 < size2)
					cmp += memcmp(ptr, g2->data_ptr+min, diff);
				else
					cmp += memcmp(g1->data_ptr+min, ptr, diff);
				free(ptr);
			}else{
				cmp += size1 - size2;
			}
		}
	}
	return cmp;
}

u32 generic_size(const generic_t *g)
{
	u32 data_size = 0;
	if(g != NULL){
		data_size = type_sizeof(g->type_name);
	}
	return data_size;
}

void generic_free(generic_t *g)
{
	func_ptr_t free_f;

	if(g != NULL){
		free_f = type_get_func(g->type_name, "free");
		if(free_f) free_f(g->data_ptr);
		else free(g->data_ptr);
		free(g->type_name);
		free(g);
	}
}

