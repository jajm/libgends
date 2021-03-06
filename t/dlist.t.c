#include <stdlib.h>
#include <stdio.h>
#include <libtap13/tap.h>
#include "dlist.h"
#include "lambda.h"

void assert_list_is(gds_dlist_t *list, size_t size, void *data[])
{
	gds_iterator_t *it;
	unsigned int i = 0;
	int verified = 1;

	it = gds_dlist_iterator_new(list);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it) && i < size) {
		void *d = gds_iterator_get(it);
		if (d != data[i]) {
			verified = 0;
			break;
		}
		i++;
	}
	if (i != size || i != gds_dlist_size(list) || gds_iterator_step(it) == 0) {
		verified = 0;
	}

	gds_iterator_free(it);

	ok(verified);
}

#define list_is(list, ...) ({ \
	void *va_args[] = {__VA_ARGS__}; \
	assert_list_is(list, sizeof(va_args) / sizeof(*va_args), va_args); \
})

void t_gds_dlist_unshift(void)
{
	gds_dlist_t *list;
	int a[] = {1, 2, 3};

	list = gds_dlist();
	isnt(list, NULL);

	list_is(list);

	gds_dlist_unshift(list, &a[0]);
	list_is(list, &a[0]);

	gds_dlist_unshift(list, &a[1]);
	list_is(list, &a[1], &a[0]);

	gds_dlist_unshift(list, &a[2]);
	list_is(list, &a[2], &a[1], &a[0]);

	gds_dlist_unshift(list, &a[0], &a[1], &a[2]);
	list_is(list, &a[0], &a[1], &a[2], &a[2], &a[1], &a[0]);

	gds_dlist_unshift(list);
	list_is(list, &a[0], &a[1], &a[2], &a[2], &a[1], &a[0]);

	gds_dlist_free(list);
}

void t_gds_dlist_push(void)
{
	gds_dlist_t *list;
	int a[] = {1, 2, 3};

	list = gds_dlist();
	isnt(list, NULL);

	list_is(list);

	gds_dlist_push(list, &a[0]);
	list_is(list, &a[0]);

	gds_dlist_push(list, &a[1]);
	list_is(list, &a[0], &a[1]);

	gds_dlist_push(list, &a[2]);
	list_is(list, &a[0], &a[1], &a[2]);

	gds_dlist_push(list, &a[2], &a[1], &a[0]);
	list_is(list, &a[0], &a[1], &a[2], &a[2], &a[1], &a[0]);

	gds_dlist_push(list);
	list_is(list, &a[0], &a[1], &a[2], &a[2], &a[1], &a[0]);

	gds_dlist_free(list);
}

void t_gds_dlist_shift(void)
{
	gds_dlist_t *list;
	int a[] = {1, 2, 3};
	int *b;

	list = gds_dlist(&a[0], &a[1], &a[2]);
	isnt(list, NULL);
	list_is(list, &a[0], &a[1], &a[2]);

	b = gds_dlist_shift(list);
	list_is(list, &a[1], &a[2]);
	is(b, &a[0]);

	b = gds_dlist_shift(list);
	list_is(list, &a[2]);
	is(b, &a[1]);

	b = gds_dlist_shift(list);
	list_is(list);
	is(b, &a[2]);

	b = gds_dlist_shift(list);
	list_is(list);
	is(b, NULL);

	gds_dlist_free(list);
}

void t_gds_dlist_pop(void)
{
	gds_dlist_t *list;
	int a[] = {1, 2, 3};
	int *b;

	list = gds_dlist(&a[0], &a[1], &a[2]);
	isnt(list, NULL);
	list_is(list, &a[0], &a[1], &a[2]);

	b = gds_dlist_pop(list);
	list_is(list, &a[0], &a[1]);
	is(b, &a[2]);

	b = gds_dlist_pop(list);
	list_is(list, &a[0]);
	is(b, &a[1]);

	b = gds_dlist_pop(list);
	list_is(list);
	is(b, &a[0]);

	b = gds_dlist_pop(list);
	list_is(list);
	is(b, NULL);

	gds_dlist_free(list);
}

void t_gds_dlist_get(void)
{
	gds_dlist_t *list;
	int a[] = {1, 2, 3};
	int i;

	list = gds_dlist(&a[0], &a[1], &a[2]);
	for (i = 0; i < 3; i++) {
		is(gds_dlist_get(list, i), &a[i]);
	}
	is(gds_dlist_get(list, i), NULL);
	gds_dlist_free(list);
}

void t_gds_dlist_splice(void)
{
	gds_dlist_t *list, *list2;
	int *a[10];
	int i;

	list = gds_dlist();
	isnt(list, NULL);
	for (i=0; i<10; i++) {
		a[i] = malloc(sizeof(int));
		*(a[i]) = i;
		gds_dlist_push(list, a[i]);
	}
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 0, 0, NULL);
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 0, 1, NULL);
	list_is(list, a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 0, 2, NULL);
	list_is(list, a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 2, 1, NULL);
	list_is(list, a[3], a[4], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 5, 1, NULL);
	list_is(list, a[3], a[4], a[6], a[7], a[8]);

	gds_dlist_splice(list, 5, 1, NULL);
	list_is(list, a[3], a[4], a[6], a[7], a[8]);

	gds_dlist_splice(list, 2, 10, NULL);
	list_is(list, a[3], a[4]);

	gds_dlist_splice(list, 0, 10, NULL);
	list_is(list);

	for (i=0; i<10; i++) {
		gds_dlist_push(list, a[i]);
	}
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	list2 = gds_dlist();
	int *b[2];
	for (i = 0; i < 2; i++) {
		b[i] = malloc(sizeof(int));
		*(b[i]) = 10+i;
		gds_dlist_push(list2, b[i]);
	}
	list_is(list2, b[0], b[1]);

	gds_dlist_splice(list, 0, 0, list2);
	list_is(list, b[0], b[1], a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 1, 0, list2);
	list_is(list, b[0], b[0], b[1], b[1], a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 0, 4, NULL);
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 5, 0, list2);
	list_is(list, a[0], a[1], a[2], a[3], a[4], b[0], b[1], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 20, 0, list2);
	list_is(list, a[0], a[1], a[2], a[3], a[4], b[0], b[1], a[5], a[6], a[7], a[8], a[9], b[0], b[1]);

	gds_dlist_splice(list, 5, 2, NULL);
	gds_dlist_splice(list, 10, 2, NULL);
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_set_free_callback(list, free);
	gds_dlist_splice(list, 0, 5, list2);
	list_is(list, b[0], b[1], a[5], a[6], a[7], a[8], a[9]);

	gds_dlist_splice(list, 0, 10, NULL);
	list_is(list);

	gds_dlist_free(list);
	gds_dlist_free(list2);
}

void t_gds_dlist_slice(void)
{
	gds_dlist_t *list, *list2;
	int *a[10];
	int *b;
	int i;

	list = gds_dlist();
	isnt(list, NULL);
	for (i=0; i<10; i++) {
		a[i] = malloc(sizeof(int));
		*(a[i]) = i;
		gds_dlist_push(list, a[i]);
	}
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	list2 = gds_dlist_slice(list, 0, 2, NULL);
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
	list_is(list2, a[0], a[1]);
	gds_dlist_free(list2);

	list2 = gds_dlist_slice(list, 2, 5, NULL);
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
	list_is(list2, a[2], a[3], a[4], a[5], a[6]);
	gds_dlist_free(list2);

	list2 = gds_dlist_slice(list, 7, 10, NULL);
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
	list_is(list2, a[7], a[8], a[9]);
	gds_dlist_free(list2);

	list2 = gds_dlist_slice(list, 2, 4, gds_lambda(int *, (int *i) {
		int *a = malloc(sizeof(int));
		*a = *i;
		return a;
	}));
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
	i = 0;
	gds_dlist_foreach(b, list2) {
		isnt(b, a[i+2], NULL);
		is(*b, *(a[i+2]));
		i++;
	}
	is(i, 4);
	gds_dlist_set_free_callback(list, free);
	gds_dlist_set_free_callback(list2, free);
	gds_dlist_free(list);
	gds_dlist_free(list2);
}

void t_gds_dlist_map(void)
{
	gds_dlist_t *list;
	int a[] = {1, 2, 3};

	list = gds_dlist(&a[0], &a[1], &a[2]);
	gds_dlist_map(list, gds_lambda(void *, (int *a) {
		*a = *a + *a;
		return a;
	}));
	is(a[0], 2);
	is(a[1], 4);
	is(a[2], 6);

	gds_dlist_map(list, gds_lambda(void *, (int *a) {
		int *b = malloc(sizeof(int));
		*b = *a + *a;
		return b;
	}));
	is(a[0], 2);
	is(a[1], 4);
	is(a[2], 6);
	is(*((int *)gds_dlist_get(list, 0)), 4);
	is(*((int *)gds_dlist_get(list, 1)), 8);
	is(*((int *)gds_dlist_get(list, 2)), 12);

	gds_dlist_set_free_callback(list, free);
	gds_dlist_free(list);
}

void t_gds_dlist_filter(void)
{
	gds_dlist_t *list, *list2;
	int *a[10];
	int i;

	list = gds_dlist_new(free);
	isnt(list, NULL);
	for (i=0; i<10; i++) {
		a[i] = malloc(sizeof(int));
		*(a[i]) = i;
		gds_dlist_push(list, a[i]);
	}
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);

	list2 = gds_dlist_filter(list, gds_lambda(int, (int *a) {
		return (*a) % 2;
	}));
	list_is(list, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
	list_is(list2, a[1], a[3], a[5], a[7], a[9]);

	gds_dlist_free(list);
	gds_dlist_destroy(list2);
}

char * string_reduce_join(char *s1, const char *s2, char *sep)
{
	if (s1 == NULL) {
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

void t_gds_dlist_reduce(void)
{
	gds_dlist_t *list;
	char *s[] = {"Hello,", "splitted", "world!"};
	char *result;
	char sep = ' ';
	int i;

	list = gds_dlist();
	for (i = 0; i < 3; i++) {
		gds_dlist_push(list, s[i]);
	}

	result = gds_dlist_reduce(list, gds_lambda(char *, (char *s1, char *s2) {
		return string_reduce_join(s1, s2, NULL);
	}));
	str_eq(result, "Hello,splittedworld!");
	free(result);
	result = gds_dlist_reduce(list, gds_lambda(char *, (char *s1, char *s2) {
		return string_reduce_join(s1, s2, &sep);
	}));
	str_eq(result, "Hello, splitted world!");
	free(result);

	gds_dlist_free(list);
}

int main()
{
	plan(91);

	t_gds_dlist_unshift();
	t_gds_dlist_push();
	t_gds_dlist_shift();
	t_gds_dlist_pop();
	t_gds_dlist_get();
	t_gds_dlist_splice();
	t_gds_dlist_slice();
	t_gds_dlist_map();
	t_gds_dlist_filter();
	t_gds_dlist_reduce();

	return EXIT_SUCCESS;
}
