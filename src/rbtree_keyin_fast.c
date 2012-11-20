#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree_fast_inline.h"
#include "rbtree_keyin_fast.h"
#include "rbtree_keyin_fast_node.h"
#include "callbacks.h"

#define rbt_containerof(ptr) \
	((ptr) != NULL) \
	? (gds_rbtree_keyin_fast_node_t *) \
		((char *)ptr - offsetof(gds_rbtree_keyin_fast_node_t, rbtree)) \
	: NULL

int32_t gds_rbtree_keyin_fast_node_cmp_key(void *key,
	gds_rbtree_fast_inline_node_t *inode,
	void *params[2])
{
	gds_rbtree_keyin_fast_node_t *node;
	gds_getkey_cb getkey_cb;
	gds_cmpkey_cb cmpkey_cb;

	node = rbt_containerof(inode);
	getkey_cb = (gds_getkey_cb) params[0];
	cmpkey_cb = (gds_cmpkey_cb) params[1];

	return cmpkey_cb(key, getkey_cb(node->data));
}

gds_rbtree_fast_inline_node_t * gds_rbtree_keyin_fast_create_node(void *data,
	gds_alloc_cb alloc_cb)
{
	gds_rbtree_keyin_fast_node_t *node;

	node = gds_rbtree_keyin_fast_node_new(data, alloc_cb);
	return &(node->rbtree);
}

void gds_rbtree_keyin_fast_set_data(gds_rbtree_fast_inline_node_t *inode,
	void *data, void *params[2])
{
	gds_rbtree_keyin_fast_node_t *node;
	gds_free_cb free_cb;
	gds_alloc_cb alloc_cb;

	node = rbt_containerof(inode);
	free_cb = (gds_free_cb) params[0];
	alloc_cb = (gds_alloc_cb) params[1];

	gds_rbtree_keyin_fast_node_set_data(node, data, alloc_cb, free_cb);
}

void gds_rbtree_keyin_fast_replace(gds_rbtree_fast_inline_node_t *inode1,
	gds_rbtree_fast_inline_node_t *inode2, gds_free_cb free_cb)
{
	gds_rbtree_keyin_fast_node_t *node1, *node2;

	node1 = rbt_containerof(inode1);
	node2 = rbt_containerof(inode2);
	if (free_cb != NULL) {
		free_cb(node1->data);
	}
	node1->data = node2->data;
	gds_rbtree_keyin_fast_node_free(node2, NULL);
}

int8_t gds_rbtree_keyin_fast_add(gds_rbtree_keyin_fast_node_t **root,
	void *data, gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb alloc_cb)
{
	gds_rbtree_fast_inline_node_t *inode;
	gds_rbtf_cmp_key_cb cmp_key_cb =
		(gds_rbtf_cmp_key_cb) gds_rbtree_keyin_fast_node_cmp_key;
	gds_rbtf_create_node_cb create_node_cb =
		(gds_rbtf_create_node_cb) gds_rbtree_keyin_fast_create_node;
	int8_t already_in_tree = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (*root == NULL) {
		*root = gds_rbtree_keyin_fast_node_new(data, alloc_cb);
		(*root)->rbtree.red = false;
	} else {
		void *cmp_key_params[] = {getkey_cb, cmpkey_cb};
		inode = &((*root)->rbtree);
		already_in_tree = gds_rbtree_fast_inline_add(&inode, data,
			getkey_cb, cmp_key_cb, &cmp_key_params, create_node_cb,
			alloc_cb);
		*root = rbt_containerof(inode);
	}

	return already_in_tree;
}

void gds_rbtree_keyin_fast_set(gds_rbtree_keyin_fast_node_t **root,
	void *data, gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb, gds_alloc_cb alloc_cb)
{
	gds_rbtree_fast_inline_node_t *inode;
	gds_rbtf_cmp_key_cb cmp_key_cb =
		(gds_rbtf_cmp_key_cb) gds_rbtree_keyin_fast_node_cmp_key;
	gds_rbtf_create_node_cb create_node_cb =
		(gds_rbtf_create_node_cb) gds_rbtree_keyin_fast_create_node;
	gds_rbtf_set_data_cb set_data_cb =
		(gds_rbtf_set_data_cb) gds_rbtree_keyin_fast_set_data;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (*root == NULL) {
		*root = gds_rbtree_keyin_fast_node_new(data, alloc_cb);
		(*root)->rbtree.red = false;
	} else {
		void *cmp_key_params[] = {getkey_cb, cmpkey_cb};
		void *set_data_params[] = {free_cb, alloc_cb};
		inode = &((*root)->rbtree);
		gds_rbtree_fast_inline_set(&inode, data, getkey_cb, cmp_key_cb,
			&cmp_key_params, create_node_cb, alloc_cb, set_data_cb,
			&set_data_params);
		*root = rbt_containerof(inode);
	}
}

gds_rbtree_keyin_fast_node_t * gds_rbtree_keyin_fast_get_node(
	gds_rbtree_keyin_fast_node_t *root, void *key, gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	void *cmp_key_params[] = {getkey_cb, cmpkey_cb};
	gds_rbtree_fast_inline_node_t *inode = NULL;
	gds_rbtf_cmp_key_cb cmp_key_cb =
		(gds_rbtf_cmp_key_cb) gds_rbtree_keyin_fast_node_cmp_key;

	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (root == NULL) {
		return NULL;
	}

	inode = gds_rbtree_fast_inline_get_node(&(root->rbtree), key,
		cmp_key_cb, &cmp_key_params);

	return rbt_containerof(inode);
}

void * gds_rbtree_keyin_fast_get(gds_rbtree_keyin_fast_node_t *root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb alloc_cb)
{
	gds_rbtree_keyin_fast_node_t *n;

	GDS_CHECK_ARG_NOT_NULL(root);

	n = gds_rbtree_keyin_fast_get_node(root, key, getkey_cb, cmpkey_cb);
	return gds_rbtree_keyin_fast_node_get_data(n, alloc_cb);
}


int8_t gds_rbtree_keyin_fast_del(gds_rbtree_keyin_fast_node_t **root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_free_cb free_cb)
{
	gds_rbtree_fast_inline_node_t *inode;
	gds_rbtf_cmp_key_cb cmp_key_cb =
		(gds_rbtf_cmp_key_cb) gds_rbtree_keyin_fast_node_cmp_key;
	gds_rbtf_replace_cb replace_cb =
		(gds_rbtf_replace_cb) gds_rbtree_keyin_fast_replace;
	int8_t not_in_tree = 1;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (*root != NULL) {
		void *cmp_key_params[] = {getkey_cb, cmpkey_cb};
		inode = &((*root)->rbtree);

		not_in_tree = gds_rbtree_fast_inline_del(&inode, key,
			cmp_key_cb, &cmp_key_params, replace_cb, free_cb);

		*root = rbt_containerof(inode);
	}

	return not_in_tree;
}

void gds_rbtree_keyin_fast_free(gds_rbtree_keyin_fast_node_t *root,
	gds_free_cb free_cb)
{
	gds_rbtree_keyin_fast_node_t *node;

	if (root != NULL) {
		node = rbt_containerof(root->rbtree.left);
		gds_rbtree_keyin_fast_free(node, free_cb);

		node = rbt_containerof(root->rbtree.right);
		gds_rbtree_keyin_fast_free(node, free_cb);

		gds_rbtree_keyin_fast_node_free(root, free_cb);
	}
}
