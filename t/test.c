#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

test_t * test_new(const char *key, int value)
{
	test_t *t;
	size_t len;

	t = malloc(sizeof(test_t));
	if (t != NULL) {
		if (key != NULL) {
			len = strlen(key);
			t->key = malloc(len+1);
			if (t->key != NULL) {
				strncpy(t->key, key, len+1);
			}
		} else {
			t->key = NULL;
		}
		t->value = value;
	}

	return t;
}

test_t * test_alloc(test_t *t)
{
	test_t *copy = NULL;

	if (t != NULL) {
		copy = test_new(t->key, t->value);
	}

	return copy;
}

char * test_getkey(test_t *t)
{
	return (t != NULL) ? t->key : NULL;
}

int test_setkey(test_t *t, const char *key)
{
	size_t len;
	char *s = NULL;

	if (t == NULL) {
		return -1;
	}
	
	if (key != NULL) {
		len = strlen(key);
		s = malloc(len+1);
		if (s == NULL)
			return -1;
		strncpy(s, key, len+1);
	}

	free(t->key);
	t->key = s;
	
	return 0;
}

int test_cmpkey(const char *key1, const char *key2)
{
	int cmp;

	if (key1 == NULL && key2 == NULL)
		cmp = 0;
	else if (key1 != NULL && key2 == NULL)
		cmp = 1;
	else if (key1 == NULL && key2 != NULL)
		cmp = -1;
	else
		cmp = strcmp(key1, key2);

	return cmp;
}

int test_getvalue(test_t *t)
{
	return (t != NULL) ? t->value : 0;
}

int test_setvalue(test_t *t, int value)
{
	if (t == NULL) {
		return -1;
	}

	t->value = value;
	return 0;
}

void test_free(test_t *t)
{
	if (t != NULL) {
		free(t->key);
		free(t);
	}
}
