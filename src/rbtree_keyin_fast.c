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
#include "rbtree_keyin_fast.h"
#include "rbtree_keyin_fast_node.h"
#include "callbacks.h"

#define rbt_containerof(ptr) \
	((ptr) != NULL) \
	? (gds_rbtree_keyin_fast_node_t *) \
		((char *)ptr - offsetof(gds_rbtree_keyin_fast_node_t, rbtree)) \
	: NULL

int32_t gds_rbtree_keyin_fast_node_cmp_key(void *key,
	gds_inline_rbtree_fast_node_t *inode,
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

gds_inline_rbtree_fast_node_t * gds_rbtree_keyin_fast_create_node(void *data)
{
	gds_rbtree_keyin_fast_node_t *node;

	node = gds_rbtree_keyin_fast_node_new(data);
	return &(node->rbtree);
}

void gds_rbtree_keyin_fast_set_data(gds_inline_rbtree_fast_node_t *inode,
	void *data, gds_free_cb free_cb)
{
	gds_rbtree_keyin_fast_node_t *node;

	node = rbt_containerof(inode);

	gds_rbtree_keyin_fast_node_set_data(node, data, free_cb);
}

void gds_rbtree_keyin_fast_replace(gds_inline_rbtree_fast_node_t *inode1,
	gds_inline_rbtree_fast_node_t *inode2, gds_free_cb free_cb)
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
	void *data, gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb)
{
	gds_inline_rbtree_fast_node_t *inode;
	gds_rbtf_cmp_key_cb cmp_key_cb =
		(gds_rbtf_cmp_key_cb) gds_rbtree_keyin_fast_node_cmp_key;
	gds_rbtf_create_node_cb create_node_cb =
		(gds_rbtf_create_node_cb) gds_rbtree_keyin_fast_create_node;
	int8_t already_in_tree = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (*root == NULL) {
		*root = gds_rbtree_keyin_fast_node_new(data);
		(*root)->rbtree.red = false;
	} else {
		void *cmp_key_params[] = {getkey_cb, cmpkey_cb};
		inode = &((*root)->rbtree);
		already_in_tree = gds_inline_rbtree_fast_add(&inode, data,
			getkey_cb, cmp_key_cb, &cmp_key_params, create_node_cb,
			NULL);
		*root = rbt_containerof(inode);
	}

	return already_in_tree;
}

void gds_rbtree_keyin_fast_set(gds_rbtree_keyin_fast_node_t **root,
	void *data, gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb)
{
	gds_inline_rbtree_fast_node_t *inode;
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
		*root = gds_rbtree_keyin_fast_node_new(data);
		(*root)->rbtree.red = false;
	} else {
		void *cmp_key_params[] = {getkey_cb, cmpkey_cb};
		inode = &((*root)->rbtree);
		gds_inline_rbtree_fast_set(&inode, data, getkey_cb, cmp_key_cb,
			&cmp_key_params, create_node_cb, NULL, set_data_cb,
			free_cb);
		*root = rbt_containerof(inode);
	}
}

gds_rbtree_keyin_fast_node_t * gds_rbtree_keyin_fast_get_node(
	gds_rbtree_keyin_fast_node_t *root, void *key, gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb)
{
	void *cmp_key_params[] = {getkey_cb, cmpkey_cb};
	gds_inline_rbtree_fast_node_t *inode = NULL;
	gds_rbtf_cmp_key_cb cmp_key_cb =
		(gds_rbtf_cmp_key_cb) gds_rbtree_keyin_fast_node_cmp_key;

	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (root == NULL) {
		return NULL;
	}

	inode = gds_inline_rbtree_fast_get_node(&(root->rbtree), key,
		cmp_key_cb, &cmp_key_params);

	return rbt_containerof(inode);
}

void * gds_rbtree_keyin_fast_get(gds_rbtree_keyin_fast_node_t *root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb)
{
	gds_rbtree_keyin_fast_node_t *n;

	GDS_CHECK_ARG_NOT_NULL(root);

	n = gds_rbtree_keyin_fast_get_node(root, key, getkey_cb, cmpkey_cb);
	return gds_rbtree_keyin_fast_node_get_data(n);
}


int8_t gds_rbtree_keyin_fast_del(gds_rbtree_keyin_fast_node_t **root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_free_cb free_cb)
{
	gds_inline_rbtree_fast_node_t *inode;
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

		not_in_tree = gds_inline_rbtree_fast_del(&inode, key,
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
