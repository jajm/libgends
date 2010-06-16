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
#include "error.h"
#include "slist.h"

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

#define MAX 20
int main()
{
	slist_t *L;
	test_t *toh = test_new(400);
	u32 i;
	test_t * tab[MAX];
	test_t *T;
	iterator_t *it;

	Error_init();
	if((types_init(0)) < 0)
		pError();
	
	type_reg("test", sizeof(test_t));
	type_reg_func("test", "free", (func_ptr_t)&test_free);

	for(i=0; i<MAX; i++){
		tab[i] = test_new(i);
	}

	L = slist_new("test");
	for(i=0; i<MAX; i++){
		slist_add_last(L, tab[i]);
	}

	it = slist_iterator_new(L);
	
	iterator_next(it);
	iterator_next(it);
	iterator_next(it);
	iterator_next(it);
	slist_add(it, toh);

	iterator_reset(it);
	while(!iterator_end(it)){
		T = (test_t *)iterator_get(it);
		printf("%s\n", T->s);
		iterator_next(it);
	}
	
	iterator_free(it);
	printf("\n");
	slist_free(L);

	types_free();

	return 0;
}

