/*
 * Copyright 2012-2013 Julian Maurice
 *
 * This file is part of libgends
 *
 * libgends is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libgends is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "inline/rbtree_fast.h"
#include "rbtree_fast.h"
#include "callbacks.h"

gds_rbtree_fast_node_t * gds_rbtree_fast_node_new(void *key, void *data)
{
	gds_rbtree_fast_node_t *n;

	n = malloc(sizeof(gds_rbtree_fast_node_t));
	if(n == NULL) {
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_rbtree_fast_node_t));
	}

	n->key = key;
	n->data = data;

	gds_inline_rbtree_fast_node_init(&(n->rbtree));

	return n;
}

void gds_rbtree_fast_node_free(gds_rbtree_fast_node_t *node,
	gds_free_cb key_free_cb, gds_free_cb free_cb)
{
	if (node) {
		if (key_free_cb != NULL) {
			key_free_cb(node->key);
		}
		if (free_cb != NULL) {
			free_cb(node->data);
		}
		free(node);
	}
}

#define rbt_containerof(ptr) \
	((ptr) != NULL) \
	? (gds_rbtree_fast_node_t *) \
		((char *)ptr - offsetof(gds_rbtree_fast_node_t, rbtree)) \
	: NULL

int gds_rbtree_fast_node_cmp(gds_inline_rbtree_fast_node_t *inode1,
	gds_inline_rbtree_fast_node_t *inode2, gds_cmpkey_cb cmpkey_cb)
{
	gds_rbtree_fast_node_t *node1, *node2;

	node1 = rbt_containerof(inode1);
	node2 = rbt_containerof(inode2);

	return cmpkey_cb(node1->key, node2->key);
}

int gds_rbtree_fast_node_cmp_with_key(gds_inline_rbtree_fast_node_t *inode,
	void *key, gds_cmpkey_cb cmpkey_cb)
{
	gds_rbtree_fast_node_t *node;

	node = rbt_containerof(inode);

	return cmpkey_cb(key, node->key);
}

int gds_rbtree_fast_add(gds_rbtree_fast_node_t **root, void *key, void *data,
	gds_cmpkey_cb cmpkey_cb)
{
	gds_inline_rbtree_fast_node_t *iroot, *inode;
	gds_rbtree_fast_node_t *node;
	gds_rbtf_cmp_cb cmp_cb = (gds_rbtf_cmp_cb) gds_rbtree_fast_node_cmp;
	gds_rbtf_cmp_with_key_cb cmp_with_key_cb
		= (gds_rbtf_cmp_with_key_cb) gds_rbtree_fast_node_cmp_with_key;
	int already_in_tree = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (*root == NULL) {
		*root = gds_rbtree_fast_node_new(key, data);
		(*root)->rbtree.red = false;
	} else {
		iroot = &((*root)->rbtree);
		inode = gds_inline_rbtree_fast_get_node(iroot, key,
			cmp_with_key_cb, cmpkey_cb);
		if (inode == NULL) {
			node = gds_rbtree_fast_node_new(key, data);
			inode = &(node->rbtree);
			gds_inline_rbtree_fast_add(&iroot, inode, cmp_cb,
				cmpkey_cb);
			*root = rbt_containerof(iroot);
		} else {
			already_in_tree = 1;
		}
	}

	return already_in_tree;
}

gds_rbtree_fast_node_t * gds_rbtree_fast_get_node(gds_rbtree_fast_node_t *root,
	const void *key, gds_cmpkey_cb cmpkey_cb)
{
	gds_inline_rbtree_fast_node_t *inode = NULL;
	gds_rbtf_cmp_with_key_cb cmp_with_key_cb =
		(gds_rbtf_cmp_with_key_cb) gds_rbtree_fast_node_cmp_with_key;

	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (root == NULL) {
		return NULL;
	}

	inode = gds_inline_rbtree_fast_get_node(&(root->rbtree), key,
		cmp_with_key_cb, cmpkey_cb);

	return rbt_containerof(inode);
}

void gds_rbtree_fast_set(gds_rbtree_fast_node_t **root, void *key,
	void *data, gds_cmpkey_cb cmpkey_cb, gds_free_cb free_cb)
{
	gds_rbtree_fast_node_t *node;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	node = gds_rbtree_fast_get_node(*root, key, cmpkey_cb);
	if (node != NULL) {
		/* Node found: the key already exists in the tree */
		if (free_cb != NULL && node->data != data) {
			free_cb(node->data);
		}
		node->data = data;
	} else {
		/* Node not found: add a new node */
		gds_rbtree_fast_add(root, key, data, cmpkey_cb);
	}
}

void * gds_rbtree_fast_get(gds_rbtree_fast_node_t *root, const void *key,
	gds_cmpkey_cb cmpkey_cb)
{
	gds_rbtree_fast_node_t *n;

	n = gds_rbtree_fast_get_node(root, key, cmpkey_cb);

	if (n == NULL) {
		return NULL;
	} else {
		return n->data;
	}
}

int gds_rbtree_fast_del(gds_rbtree_fast_node_t **root, const void *key,
	gds_cmpkey_cb cmpkey_cb, gds_free_cb key_free_cb, gds_free_cb free_cb)
{
	gds_inline_rbtree_fast_node_t *iroot, *inode;
	gds_rbtree_fast_node_t *node;
	gds_rbtf_cmp_with_key_cb cmp_with_key_cb =
		(gds_rbtf_cmp_with_key_cb) gds_rbtree_fast_node_cmp_with_key;
	int not_in_tree = 1;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (*root != NULL) {
		iroot = &((*root)->rbtree);

		inode = gds_inline_rbtree_fast_del(&iroot, key,
			cmp_with_key_cb, cmpkey_cb);
		if (inode != NULL) {
			node = rbt_containerof(inode);
			gds_rbtree_fast_node_free(node, key_free_cb, free_cb);
			not_in_tree = 0;
		}

		*root = rbt_containerof(iroot);
	}

	return not_in_tree;
}

void gds_rbtree_fast_free(gds_rbtree_fast_node_t *root, gds_free_cb key_free_cb,
	gds_free_cb free_cb)
{
	gds_rbtree_fast_node_t *node;

	if (root != NULL) {
		node = rbt_containerof(root->rbtree.left);
		gds_rbtree_fast_free(node, key_free_cb, free_cb);

		node = rbt_containerof(root->rbtree.right);
		gds_rbtree_fast_free(node, key_free_cb, free_cb);

		gds_rbtree_fast_node_free(root, key_free_cb, free_cb);
	}
}

typedef struct {
	gds_rbtree_fast_node_t *root;
	gds_iterator_t *inline_rbtree_fast_it;
} gds_rbtree_fast_iterator_data_t;

int gds_rbtree_fast_iterator_reset(gds_rbtree_fast_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_fast_it);
	data->inline_rbtree_fast_it =
		gds_inline_rbtree_fast_iterator_new(&(data->root->rbtree));

	return 0;
}

int gds_rbtree_fast_iterator_step(gds_rbtree_fast_iterator_data_t *data)
{
	return gds_iterator_step(data->inline_rbtree_fast_it);
}

void * gds_rbtree_fast_iterator_get(gds_rbtree_fast_iterator_data_t *data)
{
	gds_inline_rbtree_fast_node_t *inline_node;
	gds_rbtree_fast_node_t *node;

	inline_node = gds_iterator_get(data->inline_rbtree_fast_it);
	node = rbt_containerof(inline_node);

	return (node != NULL) ? node->data : NULL;
}

const void * gds_rbtree_fast_iterator_getkey(gds_rbtree_fast_iterator_data_t *data)
{
	gds_inline_rbtree_fast_node_t *inline_node;
	gds_rbtree_fast_node_t *node;

	inline_node = gds_iterator_get(data->inline_rbtree_fast_it);
	node = rbt_containerof(inline_node);

	return (node != NULL) ? node->key : NULL;
}

void gds_rbtree_fast_iterator_data_free(gds_rbtree_fast_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_fast_it);
	free(data);
}

gds_iterator_t * gds_rbtree_fast_iterator_new(gds_rbtree_fast_node_t *root)
{
	gds_rbtree_fast_iterator_data_t *data;
	gds_iterator_t *it;

	GDS_CHECK_ARG_NOT_NULL(root);

	data = malloc(sizeof(gds_rbtree_fast_iterator_data_t));
	if (data == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_fast_iterator_data_t));
	}

	data->root = root;
	data->inline_rbtree_fast_it = NULL;

	it = gds_iterator_new(data,
		(gds_iterator_reset_cb) gds_rbtree_fast_iterator_reset,
		(gds_iterator_step_cb) gds_rbtree_fast_iterator_step,
		(gds_iterator_get_cb) gds_rbtree_fast_iterator_get,
		(gds_iterator_getkey_cb) gds_rbtree_fast_iterator_getkey,
		(gds_free_cb) gds_rbtree_fast_iterator_data_free);

	return it;
}
