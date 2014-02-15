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

gds_rbtree_keyin_fast_node_t * gds_rbtree_keyin_fast_node_new(void *data)
{
	gds_rbtree_keyin_fast_node_t *n;

	n = malloc(sizeof(gds_rbtree_keyin_fast_node_t));
	if(n == NULL) {
		GDS_THROW(NotEnoughMemoryException, "failed to allocate %d "
			"bytes", sizeof(gds_rbtree_keyin_fast_node_t));
	}
	
	n->data = data;

	gds_inline_rbtree_fast_node_init(&(n->rbtree));
	
	return n;
}

int gds_rbtree_keyin_fast_node_set_data(gds_rbtree_keyin_fast_node_t *node,
	void *data, void *free_cb)
{
	void (*free_callback)(void *) = free_cb;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (free_callback != NULL) {
		free_callback(node->data);
	}
	node->data = data;

	return 0;
}

void * gds_rbtree_keyin_fast_node_get_data(gds_rbtree_keyin_fast_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->data;
}

void gds_rbtree_keyin_fast_node_free(gds_rbtree_keyin_fast_node_t *node,
	void *free_cb)
{
	void (*free_callback)(void *) = free_cb;

	if (node) {
		if (free_callback != NULL) {
			free_callback(node->data);
		}
		free(node);
	}
}

#define rbt_containerof(ptr) \
	((ptr) != NULL) \
	? (gds_rbtree_keyin_fast_node_t *) \
		((char *)ptr - offsetof(gds_rbtree_keyin_fast_node_t, rbtree)) \
	: NULL

int gds_rbtree_keyin_fast_node_cmp(gds_inline_rbtree_fast_node_t *inode1,
	gds_inline_rbtree_fast_node_t *inode2, void *params[2])
{
	gds_rbtree_keyin_fast_node_t *node1, *node2;
	void *(*getkey_cb)(void *);
	int (*cmpkey_cb)(void *, void *);

	node1 = rbt_containerof(inode1);
	node2 = rbt_containerof(inode2);
	getkey_cb = params[0];
	cmpkey_cb = params[1];

	return cmpkey_cb(getkey_cb(node1->data), getkey_cb(node2->data));
}

int gds_rbtree_keyin_fast_node_cmp_with_key(
	gds_inline_rbtree_fast_node_t *inode, void *key, void *params[2])
{
	gds_rbtree_keyin_fast_node_t *node;
	void *(*getkey_cb)(void *);
	int (*cmpkey_cb)(void *, void *);

	node = rbt_containerof(inode);
	getkey_cb = params[0];
	cmpkey_cb = params[1];

	return cmpkey_cb(key, getkey_cb(node->data));
}

int gds_rbtree_keyin_fast_add(gds_rbtree_keyin_fast_node_t **root,
	void *data, void *getkey_cb, void *cmpkey_cb)
{
	gds_inline_rbtree_fast_node_t *iroot, *inode;
	gds_rbtree_keyin_fast_node_t *node;
	void *cmp_params[] = {getkey_cb, cmpkey_cb};
	int already_in_tree = 0;
	void * (*getkey_callback)(void *) = getkey_cb;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (*root == NULL) {
		*root = gds_rbtree_keyin_fast_node_new(data);
		(*root)->rbtree.red = false;
	} else {
		iroot = &((*root)->rbtree);
		inode = gds_inline_rbtree_fast_get_node(iroot,
			getkey_callback(data),
			gds_rbtree_keyin_fast_node_cmp_with_key, cmp_params);
		if (inode == NULL) {
			node = gds_rbtree_keyin_fast_node_new(data);
			inode = &(node->rbtree);
			gds_inline_rbtree_fast_add(&iroot, inode,
				gds_rbtree_keyin_fast_node_cmp, &cmp_params);
			*root = rbt_containerof(iroot);
		} else {
			already_in_tree = 1;
		}
	}

	return already_in_tree;
}

int gds_rbtree_keyin_fast_set(gds_rbtree_keyin_fast_node_t **root, void *data,
	void *getkey_cb, void *cmpkey_cb, void *free_cb)
{
	gds_inline_rbtree_fast_node_t *inode, *iroot, *removed_inode;
	gds_rbtree_keyin_fast_node_t *node, *removed_node;
	void *cmp_params[] = {getkey_cb, cmpkey_cb};
	int rc = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	iroot = (*root != NULL) ? &((*root)->rbtree) : NULL;
	node = gds_rbtree_keyin_fast_node_new(data);
	inode = &(node->rbtree);
	rc = gds_inline_rbtree_fast_set(&iroot, inode,
		gds_rbtree_keyin_fast_node_cmp, &cmp_params, &removed_inode);
	*root = rbt_containerof(iroot);

	if (rc == 2 && removed_inode != NULL) {
		removed_node = rbt_containerof(removed_inode);
		free_cb = (removed_node->data != data) ? free_cb : NULL;
		gds_rbtree_keyin_fast_node_free(removed_node, free_cb);
		rc = 1;
	}

	return rc;
}

gds_rbtree_keyin_fast_node_t * gds_rbtree_keyin_fast_get_node(
	gds_rbtree_keyin_fast_node_t *root, const void *key,
	void *getkey_cb, void *cmpkey_cb)
{
	void *cmp_with_key_params[] = {getkey_cb, cmpkey_cb};
	gds_inline_rbtree_fast_node_t *inode = NULL;

	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (root == NULL) {
		return NULL;
	}

	inode = gds_inline_rbtree_fast_get_node(&(root->rbtree), key,
		gds_rbtree_keyin_fast_node_cmp_with_key, &cmp_with_key_params);

	return rbt_containerof(inode);
}

void * gds_rbtree_keyin_fast_get(gds_rbtree_keyin_fast_node_t *root,
	const void *key, void *getkey_cb, void *cmpkey_cb)
{
	gds_rbtree_keyin_fast_node_t *n;

	GDS_CHECK_ARG_NOT_NULL(root);

	n = gds_rbtree_keyin_fast_get_node(root, key, getkey_cb, cmpkey_cb);
	return gds_rbtree_keyin_fast_node_get_data(n);
}


int gds_rbtree_keyin_fast_del(gds_rbtree_keyin_fast_node_t **root,
	const void *key, void *getkey_cb, void *cmpkey_cb, void *free_cb)
{
	gds_inline_rbtree_fast_node_t *iroot, *inode;
	gds_rbtree_keyin_fast_node_t *node;
	int not_in_tree = 1;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (*root != NULL) {
		void *cmp_with_key_params[] = {getkey_cb, cmpkey_cb};
		iroot = &((*root)->rbtree);

		inode = gds_inline_rbtree_fast_del(&iroot, key,
			gds_rbtree_keyin_fast_node_cmp_with_key,
			&cmp_with_key_params);
		if (inode != NULL) {
			node = rbt_containerof(inode);
			gds_rbtree_keyin_fast_node_free(node, free_cb);
			not_in_tree = 0;
		}

		*root = rbt_containerof(iroot);
	}

	return not_in_tree;
}

void gds_rbtree_keyin_fast_free(gds_rbtree_keyin_fast_node_t *root,
	void *free_cb)
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

typedef struct {
	gds_rbtree_keyin_fast_node_t *root;
	gds_iterator_t *inline_rbtree_fast_it;
	void * (*getkey_cb)(void *);
} gds_rbtree_keyin_fast_iterator_data_t;

int gds_rbtree_keyin_fast_iterator_reset(gds_rbtree_keyin_fast_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_fast_it);
	data->inline_rbtree_fast_it =
		gds_inline_rbtree_fast_iterator_new(&(data->root->rbtree));

	return 0;
}

int gds_rbtree_keyin_fast_iterator_step(gds_rbtree_keyin_fast_iterator_data_t *data)
{
	return gds_iterator_step(data->inline_rbtree_fast_it);
}

void * gds_rbtree_keyin_fast_iterator_get(
	gds_rbtree_keyin_fast_iterator_data_t *data)
{
	gds_inline_rbtree_fast_node_t *inline_node;
	gds_rbtree_keyin_fast_node_t *node;

	inline_node = gds_iterator_get(data->inline_rbtree_fast_it);
	node = rbt_containerof(inline_node);

	return (node != NULL) ? node->data : NULL;
}

const void * gds_rbtree_keyin_fast_iterator_getkey(
	gds_rbtree_keyin_fast_iterator_data_t *data)
{
	gds_inline_rbtree_fast_node_t *inline_node;
	gds_rbtree_keyin_fast_node_t *node;
	void * (*getkey_cb)(void *);
	const void *key = NULL;

	inline_node = gds_iterator_get(data->inline_rbtree_fast_it);
	node = rbt_containerof(inline_node);

	getkey_cb = data->getkey_cb;
	if (node != NULL && getkey_cb != NULL) {
		key = getkey_cb(node->data);
	}

	return key;
}

void gds_rbtree_keyin_fast_iterator_data_free(
	gds_rbtree_keyin_fast_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_fast_it);
	free(data);
}

gds_iterator_t * gds_rbtree_keyin_fast_iterator_new(
	gds_rbtree_keyin_fast_node_t *root, void *getkey_cb)
{
	gds_rbtree_keyin_fast_iterator_data_t *data;
	gds_iterator_t *it;

	GDS_CHECK_ARG_NOT_NULL(root);

	data = malloc(sizeof(gds_rbtree_keyin_fast_iterator_data_t));
	if (data == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_keyin_fast_iterator_data_t));
	}

	data->root = root;
	data->inline_rbtree_fast_it = NULL;
	data->getkey_cb = getkey_cb;

	it = gds_iterator_new(data, gds_rbtree_keyin_fast_iterator_reset,
		gds_rbtree_keyin_fast_iterator_step,
		gds_rbtree_keyin_fast_iterator_get,
		gds_rbtree_keyin_fast_iterator_getkey,
		gds_rbtree_keyin_fast_iterator_data_free);

	return it;
}

void gds_rbtree_keyin_fast_build_values_list(gds_rbtree_keyin_fast_node_t *root,
	gds_slist_t *list)
{
	if (root != NULL) {
		gds_rbtree_keyin_fast_build_values_list(
			rbt_containerof(root->rbtree.right), list);
		gds_slist_unshift(list, root->data);
		gds_rbtree_keyin_fast_build_values_list(
			rbt_containerof(root->rbtree.left), list);
	}
}

gds_slist_t * gds_rbtree_keyin_fast_values(gds_rbtree_keyin_fast_node_t *root)
{
	gds_slist_t *list = gds_slist_new();

	gds_rbtree_keyin_fast_build_values_list(root, list);

	return list;
}
