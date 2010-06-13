#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "error.h"
#include "types.h"
#include "iterator.h"

iterator_t *iterator_new(const char *type_name, void *container)
{
	iterator_t *it;
	size_t len;

	assert(type_name != NULL);
	assert(container != NULL);

	it = malloc(sizeof(iterator_t));
	if(it){
		len = strlen(type_name);
		it->type_name = malloc(len+1);
		if(it->type_name){
			strncpy(it->type_name, type_name, len);
			it->container = container;
			it->pointer = NULL;
		}else{
			Error("Memory allocation error");
			free(it);
			it = NULL;
		}
	}else{
		Error("Memory allocation error");
	}

	return it;
}

s8 iterator_reset(iterator_t *it)
{
	func_ptr_t first;

	assert(it != NULL);
	
	first = type_get_func(it->type_name, "first");
	if(first){
		it->pointer = (void *)first(it->container);
	}else{
		ErrorP("Failed to retrieve function 'first' for type '%s'",
			it->type_name);
		return -1;
	}

	return 0;
}

s8 iterator_next(iterator_t *it)
{
	func_ptr_t next;

	assert(it != NULL);

	next = type_get_func(it->type_name, "next");
	if(next){
		it->pointer = (void *)next(it->container, it->pointer);
	}else{
		ErrorP("Failed to retrieve function 'next' for type '%s'",
			it->type_name);
		return -1;
	}

	return 0;
}

void *iterator_get(iterator_t *it)
{
	func_ptr_t get;

	assert(it != NULL);

	get = type_get_func(it->type_name, "get");
	if(get == NULL){
		ErrorP("Failed to retrieve function 'get' for type '%s'",
			it->type_name);
		return NULL;
	}

	return (void *)get(it->container, it->pointer);
}

s8 iterator_end(iterator_t *it)
{
	func_ptr_t end;

	assert(it != NULL);

	end = type_get_func(it->type_name, "end");
	if(end == NULL){
		ErrorP("Failed to retrieve function 'end' for type '%s'",
			it->type_name);
		return -1;
	}

	return end(it->container, it->pointer);
}

void iterator_free(iterator_t *it)
{
	if(it){
		free(it->type_name);
		free(it);
	}
}
