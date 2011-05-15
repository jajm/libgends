/*****************************************************************************
 * Copyright (C) 2010 Julian Maurice                                         *
 *                                                                           *
 * This file is part of libgends.                                            *
 *                                                                           *
 * libgends is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * libgends is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.         *
 *****************************************************************************/

/*****************************************************************************
 * Fichier           : funcs.c                                               *
 * Description Brève : Liste de fonctions génériques                         *
 * Auteur            : Julian Maurice                                        *
 * Créé le           : 13/03/2010                                            *
 *****************************************************************************
 * Liste de fonctions génériques (en réalité de pointeurs de fonctions de    *
 * type intptr_t (*ptr)(void *, ...) ).                                      *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "error.h"
#include "funcs.h"


func_t *func_new(const char *func_name, func_ptr_t func_ptr)
{
	func_t *f;
	size_t len;

	if(func_name == NULL || func_ptr == NULL){
		Error("Bad parameters");
		return NULL;
	}

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
	if(func == NULL){
		Error("Bad parameters");
		return NULL;
	}
	return func->name;
}

func_ptr_t func_get_ptr(func_t *func)
{
	if(func == NULL){
		Error("Bad parameters");
		return NULL;
	}

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

	if(head == NULL || func == NULL){
		Error("Bad parameters");
		return NULL;
	}

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

	if(name == NULL){
		Error("Bad parameters");
		return NULL;
	}

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

s8 funcs_del(funcs_t *head, const char *name)
{
	func_list_node_t *node, *prev = NULL;
	char *func_name;

	if(head == NULL || name == NULL){
		Error("Bad parameters");
		return -1;
	}

	node = *head;
	while(node != NULL){
		func_name = func_get_name(node->func);
		if(strcmp(func_name, name) == 0)
			break;
		prev = node;
		node = node->next;
	}

	if(node == NULL){
		Error("Function %s does not exist", name);
		return -1;
	}

	if(node != NULL){
		if(prev == NULL)
			*head = node->next;
		else
			prev->next = node->next;
		func_free(node->func);
		free(node);
	}
	return 0;
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

