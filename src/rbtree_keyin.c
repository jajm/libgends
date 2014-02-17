/*
 * Copyright 2012-2014 Julian Maurice
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
#include "malloc.h"
#include "check_arg.h"
#include "log.h"
#include "inline/rbtree.h"
#include "rbtree_keyin.h"
#include "slist.h"

gds_rbtree_keyin_node_t * gds_rbtree_keyin_node_new(void *data)
{
	gds_rbtree_keyin_node_t *node;
	
	node = gds_malloc(sizeof(gds_rbtree_keyin_node_t));

	node->data = data;
	gds_inline_rbtree_node_init(&(node->rbtree));

	return node;
}

void * gds_rbtree_keyin_node_get_data(gds_rbtree_keyin_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->data;
}

int gds_rbtree_keyin_node_set_data(gds_rbtree_keyin_node_t *node,
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

void gds_rbtree_keyin_node_free(gds_rbtree_keyin_node_t *node, void *free_cb)
{
	void (*free_callback)(void *) = free_cb;

	if(node) {
		if (free_callback != NULL) {
			free_callback(node->data);
		}
		free(node);
	}
}

#define rbt_containerof(ptr) \
	((ptr) != NULL) \
	? (gds_rbtree_keyin_node_t *)((char *)ptr - offsetof(gds_rbtree_keyin_node_t, rbtree)) \
	: NULL

typedef struct {
	void * (*getkey_cb)(void *);
	int (*cmpkey_cb)(void *, void *);
} gds_rbtree_keyin_callbacks_t;

int gds_rbtree_keyin_node_cmp_with_key(gds_inline_rbtree_node_t *inode,
	void *key, gds_rbtree_keyin_callbacks_t *callbacks)
{
	gds_rbtree_keyin_node_t *node;
	int cmp;

	node = rbt_containerof(inode);
	cmp = callbacks->cmpkey_cb(key, callbacks->getkey_cb(node->data));
	return cmp;
}

int gds_rbtree_keyin_node_cmp(gds_inline_rbtree_node_t *inode1,
	gds_inline_rbtree_node_t *inode2,
	gds_rbtree_keyin_callbacks_t *callbacks)
{
	gds_rbtree_keyin_node_t *node1, *node2;
	int cmp;

	node1 = rbt_containerof(inode1);
	node2 = rbt_containerof(inode2);

	cmp = callbacks->cmpkey_cb(callbacks->getkey_cb(node1->data),
		callbacks->getkey_cb(node2->data));

	return cmp;
}

int gds_rbtree_keyin_add(gds_rbtree_keyin_node_t **root, void *data,
	void *getkey_cb, void *cmpkey_cb)
{
	gds_rbtree_keyin_node_t *node;
	gds_rbtree_keyin_callbacks_t callbacks;
	gds_inline_rbtree_node_t *inode;
	int rc = 0;
	void * (*getkey_callback)(void *) = getkey_cb;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	callbacks.getkey_cb = getkey_cb;
	callbacks.cmpkey_cb = cmpkey_cb;

	if (*root == NULL) {
		*root = gds_rbtree_keyin_node_new(data);
		(*root)->rbtree.red = false;
	} else {
		inode = gds_inline_rbtree_get_node(&((*root)->rbtree),
			getkey_callback(data),
			gds_rbtree_keyin_node_cmp_with_key, &callbacks);
		if (inode == NULL) {
			node = gds_rbtree_keyin_node_new(data);
			inode = &((*root)->rbtree);
			rc = gds_inline_rbtree_add(&inode, &(node->rbtree),
				gds_rbtree_keyin_node_cmp, &callbacks);
			*root = rbt_containerof(inode);
		} else {
			rc = 1;
		}
	}

	return rc;
}

gds_rbtree_keyin_node_t * gds_rbtree_keyin_get_node(
	gds_rbtree_keyin_node_t *root, const void *key, void *getkey_cb,
	void *cmpkey_cb)
{
	gds_rbtree_keyin_callbacks_t callbacks;
	gds_inline_rbtree_node_t *inode = NULL;

	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (root == NULL) {
		return NULL;
	}

	callbacks.getkey_cb = getkey_cb;
	callbacks.cmpkey_cb = cmpkey_cb;

	inode = gds_inline_rbtree_get_node(&(root->rbtree), key,
		gds_rbtree_keyin_node_cmp_with_key, &callbacks);

	return rbt_containerof(inode);
}

void * gds_rbtree_keyin_get(gds_rbtree_keyin_node_t *root, const void *key,
	void *getkey_cb, void *cmpkey_cb)
{
	gds_rbtree_keyin_node_t *node;

	node = gds_rbtree_keyin_get_node(root, key, getkey_cb, cmpkey_cb);

	if (node == NULL) {
		return NULL;
	} else {
		return gds_rbtree_keyin_node_get_data(node);
	}
}

int gds_rbtree_keyin_set(gds_rbtree_keyin_node_t **root, void *data,
	void *getkey_cb, void *cmpkey_cb, void *free_cb)
{
	gds_rbtree_keyin_node_t *node, *removed;
	gds_inline_rbtree_node_t *iroot, *iremoved;
	gds_rbtree_keyin_callbacks_t callbacks
		= { .getkey_cb = getkey_cb, .cmpkey_cb = cmpkey_cb };
	int rc;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	node = gds_rbtree_keyin_node_new(data);
	iroot = (*root != NULL) ? &((*root)->rbtree) : NULL;
	rc = gds_inline_rbtree_set(&iroot, &(node->rbtree),
		gds_rbtree_keyin_node_cmp, &callbacks, &iremoved);
	*root = rbt_containerof(iroot);

	if (rc == 2 && iremoved != NULL) {
		rc = 1;
		removed = rbt_containerof(iremoved);
		free_cb = (removed->data != data) ? free_cb : NULL;
		gds_rbtree_keyin_node_free(removed, free_cb);
	}

	return rc;
}

int gds_rbtree_keyin_del(gds_rbtree_keyin_node_t **root, const void *key,
	void *getkey_cb, void *cmpkey_cb, void *free_cb)
{
	gds_inline_rbtree_node_t *iroot, *inode;
	gds_rbtree_keyin_node_t *node;
	gds_rbtree_keyin_callbacks_t callbacks;
	int deleted = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if(*root == NULL) {
		GDS_LOG_WARNING("Tree is empty");
		return 1;
	}

	callbacks.getkey_cb = getkey_cb;
	callbacks.cmpkey_cb = cmpkey_cb;

	iroot = &((*root)->rbtree);
	inode = gds_inline_rbtree_del(&iroot, key,
		gds_rbtree_keyin_node_cmp_with_key, &callbacks);
	if (inode != NULL) {
		node = rbt_containerof(inode);
		gds_rbtree_keyin_node_free(node, free_cb);
		deleted = 1;
	}
	*root = rbt_containerof(iroot);

	return deleted ? 0 : 1;
}

void * gds_rbtree_keyin_pop(gds_rbtree_keyin_node_t **root, const void *key,
	void *getkey_cb, void *cmpkey_cb)
{
	gds_inline_rbtree_node_t *iroot, *inode;
	gds_rbtree_keyin_node_t *node;
	gds_rbtree_keyin_callbacks_t callbacks;
	void *data = NULL;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if(*root == NULL) {
		GDS_LOG_WARNING("Tree is empty");
		return NULL;
	}

	callbacks.getkey_cb = getkey_cb;
	callbacks.cmpkey_cb = cmpkey_cb;

	iroot = &((*root)->rbtree);
	inode = gds_inline_rbtree_del(&iroot, key,
		gds_rbtree_keyin_node_cmp_with_key, &callbacks);
	if (inode != NULL) {
		node = rbt_containerof(inode);
		data = node->data;
		gds_rbtree_keyin_node_free(node, NULL);
	}
	*root = rbt_containerof(iroot);

	return data;
}

void gds_rbtree_keyin_free(gds_rbtree_keyin_node_t *root, void *free_cb)
{
	gds_rbtree_keyin_node_t *node;

	if (root != NULL) {
		node = rbt_containerof(root->rbtree.son[0]);
		gds_rbtree_keyin_free(node, free_cb);

		node = rbt_containerof(root->rbtree.son[1]);
		gds_rbtree_keyin_free(node, free_cb);

		gds_rbtree_keyin_node_free(root, free_cb);
	}
}

typedef struct {
	gds_rbtree_keyin_node_t *root;
	gds_iterator_t *inline_rbtree_it;
	void * (*getkey_cb)(void *);
} gds_rbtree_keyin_iterator_data_t;

int gds_rbtree_keyin_iterator_reset(gds_rbtree_keyin_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_it);
	data->inline_rbtree_it =
		gds_inline_rbtree_iterator_new(&(data->root->rbtree));

	return 0;
}

int gds_rbtree_keyin_iterator_step(gds_rbtree_keyin_iterator_data_t *data)
{
	return gds_iterator_step(data->inline_rbtree_it);
}

void * gds_rbtree_keyin_iterator_get(gds_rbtree_keyin_iterator_data_t *data)
{
	gds_inline_rbtree_node_t *inline_node;
	gds_rbtree_keyin_node_t *node;

	inline_node = gds_iterator_get(data->inline_rbtree_it);
	node = rbt_containerof(inline_node);

	return (node != NULL) ? node->data : NULL;
}

const void * gds_rbtree_keyin_iterator_getkey(gds_rbtree_keyin_iterator_data_t *data)
{
	gds_inline_rbtree_node_t *inline_node;
	gds_rbtree_keyin_node_t *node;
	void * (*getkey_cb)(void *);
	const void *key = NULL;

	inline_node = gds_iterator_get(data->inline_rbtree_it);
	node = rbt_containerof(inline_node);

	getkey_cb = data->getkey_cb;
	if (node != NULL && getkey_cb != NULL) {
		key = getkey_cb(node->data);
	}

	return key;
}

void gds_rbtree_keyin_iterator_data_free(gds_rbtree_keyin_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_it);
	free(data);
}

gds_iterator_t * gds_rbtree_keyin_iterator_new(gds_rbtree_keyin_node_t *root,
	void *getkey_cb)
{
	gds_rbtree_keyin_iterator_data_t *data;
	gds_iterator_t *it;

	GDS_CHECK_ARG_NOT_NULL(root);

	data = gds_malloc(sizeof(gds_rbtree_keyin_iterator_data_t));

	data->root = root;
	data->inline_rbtree_it = NULL;
	data->getkey_cb = getkey_cb;

	it = gds_iterator_new(data, gds_rbtree_keyin_iterator_reset,
		gds_rbtree_keyin_iterator_step, gds_rbtree_keyin_iterator_get,
		gds_rbtree_keyin_iterator_getkey,
		gds_rbtree_keyin_iterator_data_free);

	return it;
}

void gds_rbtree_keyin_build_values_list(gds_rbtree_keyin_node_t *root,
	gds_slist_t *list)
{
	if (root != NULL) {
		gds_rbtree_keyin_build_values_list(
			rbt_containerof(root->rbtree.son[1]), list);
		gds_slist_unshift(list, root->data);
		gds_rbtree_keyin_build_values_list(
			rbt_containerof(root->rbtree.son[0]), list);
	}
}

gds_slist_t * gds_rbtree_keyin_values(gds_rbtree_keyin_node_t *root)
{
	gds_slist_t *list = gds_slist_new();

	gds_rbtree_keyin_build_values_list(root, list);

	return list;
}

