#include <stdlib.h>
#include <stdio.h>
#include <libtap13/tap.h>
#include "slist.h"
#include "lambda.h"

void assert_list_equals(gds_slist_t *list, unsigned int size, void *compare[])
{
	gds_iterator_t *it;
	unsigned int i = 0;

	it = gds_slist_iterator_new(list);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it) && i < size) {
		void *d = gds_iterator_get(it);
		is(d, compare[i]);
		i++;
	}
	is(i, size);
	is(i, gds_slist_size(list));
	isnt(gds_iterator_step(it), 0, NULL);


	gds_iterator_free(it);
}

void t_gds_slist_unshift(void)
{
	gds_slist_t *list;
	int a[] = {1, 2, 3};

	list = gds_slist();
	isnt(list, NULL);

	assert_list_equals(list, 0, (void *[]){});

	gds_slist_unshift(list, &a[0]);
	assert_list_equals(list, 1, (void *[]){&a[0]});

	gds_slist_unshift(list, &a[1]);
	assert_list_equals(list, 2, (void *[]){&a[1], &a[0]});

	gds_slist_unshift(list, &a[2]);
	assert_list_equals(list, 3, (void *[]){&a[2], &a[1], &a[0]});

	gds_slist_free(list, NULL, NULL);
}

void t_gds_slist_push(void)
{
	gds_slist_t *list;
	int a[] = {1, 2, 3};

	list = gds_slist();
	isnt(list, NULL);

	assert_list_equals(list, 0, (void *[]){});

	gds_slist_push(list, &a[0]);
	assert_list_equals(list, 1, (void *[]){&a[0]});

	gds_slist_push(list, &a[1]);
	assert_list_equals(list, 2, (void *[]){&a[0], &a[1]});

	gds_slist_push(list, &a[2]);
	assert_list_equals(list, 3, (void *[]){&a[0], &a[1], &a[2]});

	gds_slist_free(list, NULL, NULL);
}

void t_gds_slist_shift(void)
{
	gds_slist_t *list;
	int a[] = {1, 2, 3};
	int *b;

	list = gds_slist(&a[0], &a[1], &a[2]);
	isnt(list, NULL);
	assert_list_equals(list, 3, (void *[]){&a[0], &a[1], &a[2]});

	b = gds_slist_shift(list);
	assert_list_equals(list, 2, (void *[]){&a[1], &a[2]});
	is(b, &a[0]);

	b = gds_slist_shift(list);
	assert_list_equals(list, 1, (void *[]){&a[2]});
	is(b, &a[1]);

	b = gds_slist_shift(list);
	assert_list_equals(list, 0, (void *[]){});
	is(b, &a[2]);

	b = gds_slist_shift(list);
	assert_list_equals(list, 0, (void *[]){});
	is(b, NULL);

	gds_slist_free(list, NULL, NULL);
}

void t_gds_slist_pop(void)
{
	gds_slist_t *list;
	int a[] = {1, 2, 3};
	int *b;

	list = gds_slist(&a[0], &a[1], &a[2]);
	isnt(list, NULL);
	assert_list_equals(list, 3, (void *[]){&a[0], &a[1], &a[2]});

	b = gds_slist_pop(list);
	assert_list_equals(list, 2, (void *[]){&a[0], &a[1]});
	is(b, &a[2]);

	b = gds_slist_pop(list);
	assert_list_equals(list, 1, (void *[]){&a[0]});
	is(b, &a[1]);

	b = gds_slist_pop(list);
	assert_list_equals(list, 0, (void *[]){});
	is(b, &a[0]);

	b = gds_slist_pop(list);
	assert_list_equals(list, 0, (void *[]){});
	is(b, NULL);

	gds_slist_free(list, NULL, NULL);
}

void t_gds_slist_get(void)
{
	gds_slist_t *list;
	int a[] = {1, 2, 3};
	int i;

	list = gds_slist(&a[0], &a[1], &a[2]);
	for (i = 0; i < 3; i++) {
		is(gds_slist_get(list, i), &a[i]);
	}
	is(gds_slist_get(list, i), NULL);
	gds_slist_free(list, NULL, NULL);
}

void t_gds_slist_splice(void)
{
	gds_slist_t *list, *list2;
	int *a[10];
	int i;

	list = gds_slist();
	isnt(list, NULL);
	for (i=0; i<10; i++) {
		a[i] = malloc(sizeof(int));
		*(a[i]) = i;
		gds_slist_push(list, a[i]);
	}
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 0, 0, NULL, NULL, NULL);
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 0, 1, NULL, NULL, NULL);
	assert_list_equals(list, 9, (void *[])
		{a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 0, 2, NULL, NULL, NULL);
	assert_list_equals(list, 7, (void *[])
		{a[3], a[4], a[5], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 2, 1, NULL, NULL, NULL);
	assert_list_equals(list, 6, (void *[])
		{a[3], a[4], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 5, 1, NULL, NULL, NULL);
	assert_list_equals(list, 5, (void *[]){a[3], a[4], a[6], a[7], a[8]});

	gds_slist_splice(list, 5, 1, NULL, NULL, NULL);
	assert_list_equals(list, 5, (void *[]){a[3], a[4], a[6], a[7], a[8]});

	gds_slist_splice(list, 2, 10, NULL, NULL, NULL);
	assert_list_equals(list, 2, (void *[]){a[3], a[4]});

	gds_slist_splice(list, 0, 10, NULL, NULL, NULL);
	assert_list_equals(list, 0, (void *[]){});

	for (i=0; i<10; i++) {
		gds_slist_push(list, a[i]);
	}
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});

	list2 = gds_slist();
	int *b[2];
	for (i = 0; i < 2; i++) {
		b[i] = malloc(sizeof(int));
		*(b[i]) = 10+i;
		gds_slist_push(list2, b[i]);
	}
	assert_list_equals(list2, 2, (void *[]){b[0], b[1]});

	gds_slist_splice(list, 0, 0, NULL, NULL, list2);
	assert_list_equals(list, 12, (void *[]){b[0], b[1], a[0], a[1], a[2],
		a[3], a[4], a[5], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 1, 0, NULL, NULL, list2);
	assert_list_equals(list, 14, (void *[]){b[0], b[0], b[1], b[1], a[0],
		a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 0, 4, NULL, NULL, NULL);
	assert_list_equals(list, 10, (void *[]){a[0], a[1], a[2], a[3], a[4],
		a[5], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 5, 0, NULL, NULL, list2);
	assert_list_equals(list, 12, (void *[]){a[0], a[1], a[2], a[3], a[4],
		b[0], b[1], a[5], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 20, 0, NULL, NULL, list2);
	assert_list_equals(list, 14, (void *[]){a[0], a[1], a[2], a[3], a[4],
		b[0], b[1], a[5], a[6], a[7], a[8], a[9], b[0], b[1]});

	gds_slist_splice(list, 5, 2, NULL, NULL, NULL);
	gds_slist_splice(list, 10, 2, NULL, NULL, NULL);
	assert_list_equals(list, 10, (void *[]){a[0], a[1], a[2], a[3], a[4],
		a[5], a[6], a[7], a[8], a[9]});

	gds_slist_splice(list, 0, 5, free, NULL, list2);
	assert_list_equals(list, 7, (void *[]){b[0], b[1], a[5], a[6], a[7],
		a[8], a[9]});

	gds_slist_splice(list, 0, 10, free, NULL, NULL);
	assert_list_equals(list, 0, (void *[]){});

	gds_slist_free(list, NULL, NULL);
	gds_slist_free(list2, NULL, NULL);
}

void t_gds_slist_slice(void)
{
	gds_slist_t *list, *list2;
	int *a[10];
	int *b;
	int i;

	list = gds_slist();
	isnt(list, NULL);
	for (i=0; i<10; i++) {
		a[i] = malloc(sizeof(int));
		*(a[i]) = i;
		gds_slist_push(list, a[i]);
	}
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});

	list2 = gds_slist_slice(list, 0, 2, NULL, NULL);
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});
	assert_list_equals(list2, 2, (void *[]){a[0], a[1]});
	gds_slist_free(list2, NULL, NULL);

	list2 = gds_slist_slice(list, 2, 5, NULL, NULL);
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});
	assert_list_equals(list2, 5, (void *[]){a[2], a[3], a[4], a[5], a[6]});
	gds_slist_free(list2, NULL, NULL);

	list2 = gds_slist_slice(list, 7, 10, NULL, NULL);
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});
	assert_list_equals(list2, 3, (void *[]){a[7], a[8], a[9]});
	gds_slist_free(list2, NULL, NULL);

	list2 = gds_slist_slice(list, 2, 4, gds_lambda(int *, (int *i) {
		int *a = malloc(sizeof(int));
		*a = *i;
		return a;
	}), NULL);
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});
	i = 0;
	gds_slist_foreach(b, list2) {
		isnt(b, a[i+2], NULL);
		is(*b, *(a[i+2]));
		i++;
	}
	is(i, 4);
	gds_slist_free(list, free, NULL);
	gds_slist_free(list2, free, NULL);
}

void t_gds_slist_map(void)
{
	gds_slist_t *list;
	int a[] = {1, 2, 3};

	list = gds_slist(&a[0], &a[1], &a[2]);
	gds_slist_map(list, gds_lambda(void, (int *a) {
		*a = *a + *a;
	}), NULL);
	is(a[0], 2);
	is(a[1], 4);
	is(a[2], 6);

	gds_slist_free(list, NULL, NULL);
}

void t_gds_slist_filter(void)
{
	gds_slist_t *list, *list2;
	int *a[10];
	int i;

	list = gds_slist();
	isnt(list, NULL);
	for (i=0; i<10; i++) {
		a[i] = malloc(sizeof(int));
		*(a[i]) = i;
		gds_slist_push(list, a[i]);
	}
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});

	list2 = gds_slist_filter(list, gds_lambda(int, (int *a) {
		return (*a) % 2;
	}), NULL);
	assert_list_equals(list, 10, (void *[])
		{a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]});
	assert_list_equals(list2, 5, (void *[]){a[1], a[3], a[5], a[7], a[9]});

	gds_slist_free(list, free, NULL);
	gds_slist_free(list2, NULL, NULL);
}

char * string_reduce_join(char *s1, const char *s2, unsigned int i, char *sep)
{
	if (i == 0) {
		s1 = calloc(strlen(s2) + 1, sizeof(char));
	} else {
		size_t s1len = strlen(s1);
		s1 = realloc(s1, (s1len + strlen(s2) + 2) * sizeof(char));
		if (sep) {
			s1[s1len] = *sep;
			s1[s1len+1] = '\0';
		}
	}
	strcat(s1, s2);
	return s1;
}

void t_gds_slist_reduce(void)
{
	gds_slist_t *list;
	char *s[] = {"Hello,", "splitted", "world!"};
	char *result;
	char sep = ' ';
	int i;

	list = gds_slist();
	for (i = 0; i < 3; i++) {
		gds_slist_push(list, s[i]);
	}

	result = gds_slist_reduce(list, string_reduce_join, NULL);
	str_eq(result, "Hello,splittedworld!");
	free(result);
	result = gds_slist_reduce(list, string_reduce_join, &sep);
	str_eq(result, "Hello, splitted world!");
	free(result);

	gds_slist_free(list, NULL, NULL);
}

int main()
{
	plan(431);

	t_gds_slist_unshift();
	t_gds_slist_push();
	t_gds_slist_shift();
	t_gds_slist_pop();
	t_gds_slist_get();
	t_gds_slist_splice();
	t_gds_slist_slice();
	t_gds_slist_map();
	t_gds_slist_filter();
	t_gds_slist_reduce();

	return EXIT_SUCCESS;
}
