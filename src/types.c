#include <stdlib.h>
#include <string.h>
#include "types.h"

struct type_t{
	char *type_name;
	u32 type_size;
	struct type_t *next;
};

#define HT_SIZE 1000

struct type_t * g_types[HT_SIZE];

u32 type_hash(const char *type_name)
{
	u32 hash = 0;
	u32 i, size;

	size = strlen(type_name);
	for(i=0; i<size; i++){
		hash += type_name[i];
	}
	hash %= HT_SIZE;

	return hash;
}

void type_init(void)
{
	u32 i;

	for (i=0; i<HT_SIZE; i++)
		g_types[i] = NULL;
	
	/* Types par défaut */
	type_reg("u8", sizeof(u8));
	type_reg("u16", sizeof(u16));
	type_reg("u32", sizeof(u32));
	type_reg("u64", sizeof(u64));
	type_reg("s8", sizeof(s8));
	type_reg("s16", sizeof(s16));
	type_reg("s32", sizeof(s32));
	type_reg("s64", sizeof(s64));
	type_reg("float", sizeof(float));
	type_reg("double", sizeof(double));
	type_reg("char", sizeof(char));
	type_reg("string", sizeof(char *));
}

s8 type_reg(const char *type_name, u32 type_size)
{
	u32 hash;
	size_t length;
	struct type_t *tmp, *tmp2;

	if(type_name == NULL || type_size == 0)
		return -1;
	
	hash = type_hash(type_name);
	length = strlen(type_name);
	
	if(g_types[hash] == NULL){
		g_types[hash] = malloc(sizeof(struct type_t));
		if(g_types[hash]){
			g_types[hash]->type_name = malloc(length+1);
			if(g_types[hash]->type_name){
				strncpy(g_types[hash]->type_name, type_name, length+1);
			}else{
				free(g_types[hash]);
				return -1;
			}
			g_types[hash]->type_size = type_size;
		}else{
			return -1;
		}
	}else{
		tmp = g_types[hash];
		while(tmp != NULL){
			tmp2 = tmp;
			if(strcmp(tmp->type_name, type_name) == 0)
				return -2;
			tmp = tmp->next;
		}
		tmp2->next = malloc(sizeof(struct type_t));
		if(tmp2->next){
			tmp2->type_name = malloc(length+1);
			if(tmp2->type_name){
				strncpy(tmp2->type_name, type_name, length+1);
			}else{
				free(tmp2->next);
				return -1;
			}
			tmp2->type_size = type_size;
		}else{
			return -1;
		}
	}
	return 0;
}

s8 type_unreg(const char *type_name)
{
	u32 hash;
	size_t length;
	struct type_t *tmp, *tmp2;

	if(type_name == NULL) return -1;

	hash = type_hash(type_name);
	if(g_types[hash] == NULL) return -1;

	length = strlen(type_name);
	if(length == 0) return -1;

	if(strcmp(g_types[hash]->type_name, type_name) == 0){
		tmp = g_types[hash]->next;
		free(g_types[hash]);
		g_types[hash] = tmp;
	}else{
		tmp2 = g_types[hash];
		tmp = g_types[hash]->next;
		while(tmp != NULL){
			if(strcmp(tmp->type_name, type_name) == 0){
				tmp2->next = tmp->next;
				free(tmp);
				break;
			}
			tmp2 = tmp;
			tmp = tmp->next;
		}
	}
	return 0;
}

u32 type_sizeof(const char *type_name)
{
	u32 hash;
	u32 type_size = 0;
	struct type_t *tmp;

	if(type_name == NULL) return 0;

	hash = type_hash(type_name);
	tmp = g_types[hash];

	while(tmp != NULL){
		if(strcmp(tmp->type_name, type_name) == 0){
			type_size = tmp->type_size;
			break;
		}
		tmp = tmp->next;
	}

	return type_size;
}
