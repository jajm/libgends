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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include "error.h"
#include "dlist.h"
#include "stack.h"

typedef struct{
	int a;
	char *s;
} test_t;

test_t *test_new(int a)
{
	test_t *t = malloc(sizeof(test_t));
	t->a = a;
	t->s = malloc(512*sizeof(char));
	sprintf(t->s, "a = %d", t->a);
	return t;
}

void test_free(test_t *t)
{
	if(t){
		free(t->s);
		free(t);
	}
	printf("Correctly freed\n");
}

s32 test_cmp(test_t *t1, test_t *t2)
{
	if(t1 && t2){
		return t1->a - t2->a;
	}
	return 0;
}

u32 tesf(void *v, int b, int c)
{
	int *a = v;
	printf("%d %d\n", c, INT32_MAX);
	return *a+b+c;
}

void print_list(dlist_t *l)
{
	iterator_t *it;
	test_t *t;

	assert(l != NULL);

	it = dlist_iterator_new(l);
	if(iterator_reset(it) < 0) pError();
	while(iterator_has_next(it)){
		t = (test_t*) iterator_get(it);
		printf("%d -> ", t->a);
		iterator_next(it);
	}
	printf("NULL\n");
	iterator_free(it);
}

void print_stack(stack_t *S)
{
	slist_node_t *node;
	test_t *t;

	node = S->head;
	puts("Pile");
	puts("----");
	while(node != NULL){
		t = (test_t *)slnode_data(node);
		printf("%d\n", t->a);
		node = node->next;
	}
	printf("\n\n");
}

#define MAX 20
int main()
{
	dlist_t *L;
	stack_t *S;
	//test_t *toh = test_new(400);
	u32 i;
	test_t * tab[MAX];
	test_t *T;
	iterator_t *it;
	dlist_node_t *node;

	Error_init();
	if((types_init(0)) < 0)
		pError();
	
	type_reg("test", sizeof(test_t));
	type_reg_func("test", "free", (func_ptr_t)&test_free);
	type_reg_func("test", "cmp", (func_ptr_t)&test_cmp);
	
	for(i=0; i<MAX; i++)
		tab[i] = test_new(i);

	puts("Creation d'une nouvelle liste");
	L = dlist_new("test");
	if(L == NULL) pError();

	if(dlist_empty(L)){
		puts("La liste est vide, GOOD");
	}else{
		puts("ERROR: la liste n'est pas vide");
	}
	
	puts("Ajout de 0, 1, 2 en premiere position");
	dlist_add_first(L, tab[0]);
	dlist_add_first(L, tab[1]);
	node = dlist_add_first(L, tab[2]);
	print_list(L);

	puts("Ajout de 3, 4, 5 en derniere position");
	if(dlist_add_last(L, tab[3]) == NULL)
		pError();
	dlist_add_last(L, tab[4]);
	dlist_add_last(L, tab[5]);
	print_list(L);

	puts("Ajout de 6, 7, 8 après 2");
	dlist_add_after(L, node, tab[6]);
	dlist_add_after(L, node, tab[7]);
	dlist_add_after(L, node, tab[8]);
	print_list(L);

	it = dlist_iterator_new(L);
	if(it == NULL) pError();
	if(iterator_next(it) < 0) pError();
	iterator_next(it);
	T = (test_t*) iterator_get(it);
	printf("Ajout de 9, 10, 11 après l'itérateur (%d)\n", T->a);
	dlist_it_add_after(it, tab[9]);
	dlist_it_add_after(it, tab[10]);
	node = dlist_it_add_after(it, tab[11]);
	print_list(L);

	printf("POP du premier nœud... ");
	T = (test_t *) dlist_pop_first(L);
	printf("%d\n", T->a);
	test_free(T);
	print_list(L);

	printf("POP du dernier nœud... ");
	T = (test_t *) dlist_pop_last(L);
	printf("%d\n", T->a);
	test_free(T);
	print_list(L);

	printf("POP de l'itérateur... ");
	T = (test_t *) dlist_it_pop(it);
	printf("%d\n", T->a);
	test_free(T);
	print_list(L);

	printf("POP du nœud 11... ");
	T = (test_t *) dlist_pop(L, node);
	printf("%d\n", T->a);
	test_free(T);
	print_list(L);

	puts("Suppression du premier nœud");
	dlist_del_first(L);
	print_list(L);

	puts("Suppression du dernier nœud");
	dlist_del_last(L);
	print_list(L);

	iterator_reset(it);
	iterator_next(it);
	puts("Suppression de l'itérateur");
	dlist_it_del(it);
	print_list(L);

	printf("Premier : %d\n", ((test_t*)dlist_get_first(L))->a);
	printf("Dernier : %d\n", ((test_t*)dlist_get_last(L))->a);
	printf("Iterateur : %d\n", ((test_t*)dlist_it_get(it))->a);

	puts("Suppression du nœud 0, s'il existe");
	node = dlist_chk(L, tab[0]);
	dlist_del(L, node);
	print_list(L);

	puts("Suppression de la liste");
	dlist_free(L);

	puts("Création de la pile");
	S = stack_new("test");
	print_stack(S);
	puts("PUSH de 12");
	stack_push(S, tab[12]);
	print_stack(S);
	puts("POP 2 fois");
	test_free((test_t *)stack_pop(S));
	stack_pop(S);
	print_stack(S);
	puts("PUSH 13, 14");
	stack_push(S, tab[13]);
	stack_push(S, tab[14]);
	print_stack(S);
	puts("Libération de la pile");
	stack_free(S);
	
	puts("Libération mémoire");
	for(i=15; i<MAX; i++)
		test_free(tab[i]);

	types_free();

	return 0;
}

