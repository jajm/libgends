#include <stdio.h>
#include <stdlib.h>

#include "ll_hash_map.h"
#include "ll_slist.h"

uint32_t myhash_cb(int8_t i)
{
	return i;
}

int8_t mygetkey_cb(int8_t i)
{
	return i;
}

int32_t mycmpkey_cb(int8_t a, int8_t b)
{
	return (a - b);
}

int main()
{
	gds_hash_cb hash_cb = (gds_hash_cb) &myhash_cb;
	gds_getkey_cb getkey_cb = (gds_getkey_cb) &mygetkey_cb;
	gds_cmpkey_cb cmpkey_cb = (gds_cmpkey_cb) &mycmpkey_cb;
	gds_ll_hash_map_t *h;
	gds_slist_node_t *l;
	gds_iterator_t *it;

	h = gds_ll_hash_map_new(8);

	printf("Inserting values from 1 to 15...\n");
	for (intptr_t i = 1; i < 16; i++) {
		gds_ll_hash_map_set(h, hash_cb, (void *)i, getkey_cb, cmpkey_cb,
			NULL, NULL);
	}
	printf("done.\n");

	printf("Values in the hash are:\n");
	l = gds_ll_hash_map_values(h, NULL);
	it = gds_ll_slist_iterator_new(l);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it)) {
		intptr_t i = (intptr_t) gds_iterator_get(it);
		printf("%lu ", i);
	}
	printf("\n");
	gds_ll_slist_iterator_free(it);
	gds_ll_slist_free(l, NULL);

	printf("Resizing hash...\n");
	gds_ll_hash_map_change_size(h, 2, hash_cb, getkey_cb, cmpkey_cb);
	printf("done.\n");

	printf("Values in the hash are:\n");
	l = gds_ll_hash_map_values(h, NULL);
	it = gds_ll_slist_iterator_new(l);
	gds_iterator_reset(it);
	while (!gds_iterator_step(it)) {
		intptr_t i = (intptr_t) gds_iterator_get(it);
		printf("%lu ", i);
	}
	printf("\n");
	gds_ll_slist_iterator_free(it);
	gds_ll_slist_free(l, NULL);

	return 0;
}
