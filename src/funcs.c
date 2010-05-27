#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "error.h"
#include "funcs.h"

func_t *func_new(const char *func_name, func_ptr_t func_ptr)
{
	func_t *f;
	size_t len;
	
	assert(func_name != NULL);
	assert(func_ptr != NULL);
	
	f = malloc(sizeof(func_t));
	if(f){
		len = strlen(func_name);
		f->name = malloc(len+1);
		if(f->name){
			strncpy(f->name, func_name, len+1);
			f->ptr = func_ptr;
		}else{
			free(f);
			f = NULL;
		}
	}
	
	return f;
}

char *func_get_name(func_t *func)
{
	assert(func != NULL);
	
	return func->name;
}

func_ptr_t func_get_ptr(func_t *func)
{
	assert(func != NULL);
	
	return func->ptr;
}

void func_free(func_t *func)
{
	if(func){
		free(func->name);
		free(func);
	}
}


func_list_node_t *funcs_add(funcs_t *head, func_t *func)
{
	func_list_node_t *newnode;
	
	assert(head != NULL);
	assert(func != NULL);
	
	newnode = malloc(sizeof(func_list_node_t));
	if(newnode){
		newnode->func = func;
		newnode->next = *head;
		*head = newnode;
	}
	
	return newnode;
}

func_t *funcs_get(funcs_t head, const char *name)
{
	func_list_node_t *node;
	char *func_name;
	
	assert(name != NULL);
	
	node = head;
	while(node != NULL){
		func_name = func_get_name(node->func);
		if(strcmp(func_name, name) == 0)
			break;
		node = node->next;
	}

	if(node == NULL) return NULL;
	return node->func;
}

void funcs_del(funcs_t *head, const char *name)
{
	func_list_node_t *node, *prev = NULL;
	char *func_name;
	
	assert(head != NULL);
	assert(name != NULL);
	
	node = *head;
	while(node != NULL){
		func_name = func_get_name(node->func);
		if(strcmp(func_name, name) == 0)
			break;
		prev = node;
		node = node->next;
	}
	
	if(node != NULL){
		if(prev == NULL)
			*head = node->next;	
		else
			prev->next = node->next;
		func_free(node->func);
		free(node);
	}
}

void funcs_free(funcs_t head)
{
	func_list_node_t *node, *next;
	
	node = head;
	while(node != NULL){
		next = node->next;
		func_free(node->func);
		free(node);
		node = next;
	}
}

func_ptr_t funcs_get_ptr(funcs_t funcs, const char *name)
{
	func_t *func;
	func_ptr_t func_ptr = NULL;
	
	func = funcs_get(funcs, name);
	if(func != NULL) func_ptr = func_get_ptr(func);
	
	return func_ptr;
}
