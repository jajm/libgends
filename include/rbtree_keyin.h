#ifndef rbtree_keyin_h_included
#define rbtree_keyin_h_included

#include <stdint.h>
#include "rbtree_keyin_node.h"
#include "callbacks.h"
#include "slist_node.h"

int8_t
gds_rbtree_keyin_add(
	gds_rbtree_keyin_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb
);

int8_t
gds_rbtree_keyin_set(
	gds_rbtree_keyin_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

void *
gds_rbtree_keyin_get(
	gds_rbtree_keyin_node_t *root,
	void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb
);

int8_t
gds_rbtree_keyin_del(
	gds_rbtree_keyin_node_t **root,
	void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

void
gds_rbtree_keyin_free(
	gds_rbtree_keyin_node_t *root,
	gds_free_cb free_cb
);

gds_slist_node_t *
gds_rbtree_keyin_values(
	gds_rbtree_keyin_node_t *root
);

#endif /* Not defined rbtree_keyin_h_included */

