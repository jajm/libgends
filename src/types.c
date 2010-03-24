#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define DEFAULT_HT_SIZE	1000

struct type_t{
	char *type_name;
	u32 type_size;
	struct type_t *next;
};

struct types_t{
	struct type_t **types;
	u32 size;
} *g_types = NULL;

u32 type_hash(const char *type_name)
{
	u32 hash = 0;
	u32 i, size;

	if(g_types == NULL) return 0;

	size = strlen(type_name);
	for(i=0; i<size; i++){
		hash += type_name[i];
	}
	hash %= g_types->size;

	return hash;
}

s8 types_init(u32 size, error_ptr *err)
{
	u32 i;
	
	if(g_types == NULL){
		g_types = malloc(sizeof(struct types_t));
		if(g_types == NULL){
			error_set(err, -1, "Memory allocation error");
			return -1;
		}
	}else{
		error_set(err, -1, "User types already initialized");
		return -1;
	}
	
	g_types->size = (size == 0) ? DEFAULT_HT_SIZE : size;
	g_types->types = malloc(g_types->size*sizeof(struct type_t *));
	if(g_types->types == NULL){
		error_set(err, -1, "Memory allocation error");
		free(g_types);
		return -1;
	}

	for (i=0; i<g_types->size; i++)
		g_types->types[i] = NULL;
	
	/* Types par défaut */
	type_reg("u8", sizeof(u8), NULL);
	type_reg("u16", sizeof(u16), NULL);
	type_reg("u32", sizeof(u32), NULL);
	type_reg("u64", sizeof(u64), NULL);
	type_reg("s8", sizeof(s8), NULL);
	type_reg("s16", sizeof(s16), NULL);
	type_reg("s32", sizeof(s32), NULL);
	type_reg("s64", sizeof(s64), NULL);
	type_reg("float", sizeof(float), NULL);
	type_reg("double", sizeof(double), NULL);
	type_reg("char", sizeof(char), NULL);

	return 0;
}

s8 type_reg(const char *type_name, u32 type_size, error_ptr *err)
{
	u32 hash;
	size_t length;
	struct type_t *tmp, *tmp2;

	if(type_name == NULL || type_size == 0){
		error_set(err, -1, "Bad parameters");
		return -1;
	}
	if(g_types == NULL){
		error_set(err, -1, "User types not initialized");
		return -1;
	}
	
	hash = type_hash(type_name);
	length = strlen(type_name);
	
	if(g_types->types[hash] == NULL){
		g_types->types[hash] = malloc(sizeof(struct type_t));
		if(g_types->types[hash]){
			g_types->types[hash]->type_name = malloc(length+1);
			if(g_types->types[hash]->type_name){
				strncpy(g_types->types[hash]->type_name, type_name, length+1);
			}else{
				error_set(err, -1, "Memory allocation error");
				free(g_types->types[hash]);
				return -1;
			}
			g_types->types[hash]->type_size = type_size;
		}else{
			error_set(err, -1, "Memory allocation error");
			return -1;
		}
	}else{
		tmp = g_types->types[hash];
		while(tmp != NULL){
			tmp2 = tmp;
			if(strcmp(tmp->type_name, type_name) == 0){
				error_set(err, -1, "Type '%s' already exists",
					type_name);
				return -1;
			}
			tmp = tmp->next;
		}
		tmp2->next = malloc(sizeof(struct type_t));
		if(tmp2->next){
			tmp2->next->type_name = malloc(length+1);
			if(tmp2->next->type_name){
				strncpy(tmp2->next->type_name, type_name, length+1);
			}else{
				error_set(err, -1, "Memory allocation error");
				free(tmp2->next);
				return -1;
			}
			tmp2->next->type_size = type_size;
		}else{
			error_set(err, -1, "Memory allocation error");
			return -1;
		}
	}

	error_free(err);
	return 0;
}

s8 type_unreg(const char *type_name, error_ptr *err)
{
	u32 hash;
	size_t length;
	struct type_t *tmp, *tmp2;

	if(g_types == NULL){
		error_set(err, -1, "User types not initialized");
		return -1;
	}

	if(type_name == NULL){
		error_set(err, -1, "Bad parameters");
		return -1;
	}

	length = strlen(type_name);
	if(length == 0){
		error_set(err, -1, "Zero-length string");
		return -1;
	}

	hash = type_hash(type_name);
	if(g_types->types[hash] == NULL){
		error_set(err, -1, "Type '%s' doesn't exist", type_name);
		return -1;
	}

	if(strcmp(g_types->types[hash]->type_name, type_name) == 0){
		tmp = g_types->types[hash]->next;
		free(g_types->types[hash]);
		g_types->types[hash] = tmp;
	}else{
		tmp2 = g_types->types[hash];
		tmp = g_types->types[hash]->next;
		while(tmp != NULL){
			if(strcmp(tmp->type_name, type_name) == 0){
				tmp2->next = tmp->next;
				free(tmp);
				break;
			}
			tmp2 = tmp;
			tmp = tmp->next;
		}
		if(tmp == NULL){
			error_set(err, -1, "Type '%s' doesn't exist", type_name);
			return -1;
		}
	}

	error_free(err);
	return 0;
}

u32 type_sizeof(const char *type_name)
{
	u32 hash;
	u32 type_size = 0;
	struct type_t *tmp;

	if(g_types == NULL || type_name == NULL) return 0;

	hash = type_hash(type_name);
	tmp = g_types->types[hash];

	while(tmp != NULL){
		if(strcmp(tmp->type_name, type_name) == 0){
			type_size = tmp->type_size;
			break;
		}
		tmp = tmp->next;
	}

	return type_size;
}

void types_free(void)
{
	u32 i;
	struct type_t *tmp, *tmp2;

	if(g_types == NULL) return;

	for(i=0; i<g_types->size; i++){
		if(g_types->types[i] != NULL){
			tmp = g_types->types[i];
			while(tmp != NULL){
				tmp2 = tmp->next;
				free(tmp);
				tmp = tmp2;
			}
			g_types->types[i] = NULL;
		}
	}
	free(g_types);
	g_types = NULL;
}


void types_print(void)
{
	u32 i;
	struct type_t *tmp;

	if(g_types == NULL){
		printf("User types not initialized\n");
		return;
	}

	for(i=0; i<g_types->size; i++){
		tmp = g_types->types[i];
		if(tmp) printf("[%d]", i);
		while(tmp != NULL){
			printf("\ttype_name(\"%s\"), type_size(\"%d\"), "
				"sizeof(\"%d\")\n",
				tmp->type_name, tmp->type_size,
				type_sizeof(tmp->type_name));
			tmp = tmp->next;
		}
	}
}
