#ifndef gds_rbtree_h_included
#define gds_rbtree_h_included

#include <stdint.h>
#include "rbtree_node.h"
#include "callbacks.h"
#include "slist.h"

int8_t
gds_rbtree_add(
	gds_rbtree_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb
);

int8_t
gds_rbtree_set(
	gds_rbtree_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

void *
gds_rbtree_get(
	gds_rbtree_node_t *root,
	void *key,
	gds_cmpkey_cb cmpkey_cb
);

int8_t
gds_rbtree_del(
	gds_rbtree_node_t **root,
	void *key,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

void
gds_rbtree_free(
	gds_rbtree_node_t *root,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

gds_slist_t *
gds_rbtree_keys(
	gds_rbtree_node_t *root
);

gds_slist_t *
gds_rbtree_values(
	gds_rbtree_node_t *root
);

/* Return a list of gds_key_value_t (see key_value.h) */
/* Free with gds_slist_free(list, (gds_free_cb)free) */
gds_slist_t *
gds_rbtree_keys_values(
	gds_rbtree_node_t *root
);

#endif /* Not defined gds_rbtree_h_included */

