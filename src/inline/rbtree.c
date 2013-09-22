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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "iterator.h"
#include "slist.h"
#include "inline/rbtree.h"
#include "../check_arg.h"

void gds_inline_rbtree_print_dbg_r(gds_inline_rbtree_node_t *root, uint8_t d)
{
	if (root != NULL) {
		gds_inline_rbtree_print_dbg_r(root->son[1], d+1);
		for (uint8_t i=0; i<d; i++)
			printf("- ");
		printf("rbtree %p [%s] (0: %p, 1: %p)\n", root,
			root->red ? "red" : "black", root->son[0], root->son[1]);
		gds_inline_rbtree_print_dbg_r(root->son[0], d+1);
	}
}
void gds_inline_rbtree_print_dbg(gds_inline_rbtree_node_t *root)
{
	printf("\n---------------------------------------------------------\n");
	gds_inline_rbtree_print_dbg_r(root, 0);
	printf("---------------------------------------------------------\n");
}

_Bool gds_inline_rbtree_node_is_red(gds_inline_rbtree_node_t *node)
{
	return (node && node->red);
}

gds_inline_rbtree_node_t * gds_inline_rbtree_rotate(
	gds_inline_rbtree_node_t *node, uint8_t dir)
{
	gds_inline_rbtree_node_t *tmp;

	dir = (dir) ? 1 : 0;
	tmp = node->son[!dir];

	node->son[!dir] = tmp->son[dir];
	tmp->son[dir] = node;

	node->red = true;
	tmp->red = false;

	return tmp;
}

gds_inline_rbtree_node_t * gds_inline_rbtree_rotate_twice(
	gds_inline_rbtree_node_t *node, uint8_t dir)
{
	node->son[!dir] = gds_inline_rbtree_rotate(node->son[!dir], !dir);
	return gds_inline_rbtree_rotate(node, dir);
}

void gds_inline_rbtree_node_init(gds_inline_rbtree_node_t *node)
{
	node->red = true;
	node->son[0] = node->son[1] = NULL;
}

int8_t gds_inline_rbtree_add(gds_inline_rbtree_node_t **root,
	gds_inline_rbtree_node_t *node, gds_rbt_cmp_cb rbt_cmp_cb,
	void *rbt_cmp_data)
{
	gds_inline_rbtree_node_t head;      /* False tree root */
	gds_inline_rbtree_node_t *g, *t;    /* Grandparent & parent */
	gds_inline_rbtree_node_t *p, *q;    /* Iterator & parent */
	uint8_t dir = 0, last = 0;
	int32_t cmp;
	int8_t added = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(*root);
	GDS_CHECK_ARG_NOT_NULL(node);
	GDS_CHECK_ARG_NOT_NULL(rbt_cmp_cb);

	/* Root node should be black */
	(*root)->red = false;

	/* Set up helpers */
	t = &head;
	g = p = NULL;
	q = t->son[1] = *root;

	/* Search down the tree */
	while (true) {
		if (q == NULL) {
			/* Insert new node at the bottom */
			p->son[dir] = q = node;
			added = 1;
		}
		else if (gds_inline_rbtree_node_is_red(q->son[0])
		&& gds_inline_rbtree_node_is_red(q->son[1])) {
			/* Color flip */
			q->red = true;
			q->son[0]->red = false;
			q->son[1]->red = false;
		}

		/* Fix red violation */
		if (gds_inline_rbtree_node_is_red(q)
		&& gds_inline_rbtree_node_is_red(p)) {
			uint8_t dir2 = (t->son[1] == g) ? 1 : 0;

			if (q == p->son[last]) {
				t->son[dir2] = gds_inline_rbtree_rotate(g, !last);
			} else {
				t->son[dir2] = gds_inline_rbtree_rotate_twice(g, !last);
			}
		}

		/* Stop if found */
		cmp = rbt_cmp_cb(node, q, rbt_cmp_data);
		if (cmp == 0)
			break;

		last = dir;
		dir = (cmp > 0) ? 1 : 0;

		/* Update helpers */
		if (g != NULL)
			t = g;
		g = p, p = q;
		q = q->son[dir];
	}

	/* Update root */
	*root = head.son[1];
	(*root)->red = false;

	return added;
}

gds_inline_rbtree_node_t * gds_inline_rbtree_get_node(
	gds_inline_rbtree_node_t *root, const void *key,
	gds_rbt_cmp_with_key_cb rbt_cmp_with_key_cb, void *rbt_cmp_data)
{
	gds_inline_rbtree_node_t *node;
	int32_t cmp;
	int8_t dir;

	GDS_CHECK_ARG_NOT_NULL(rbt_cmp_with_key_cb);

	node = root;
	while (node != NULL) {
		cmp = rbt_cmp_with_key_cb(node, key, rbt_cmp_data);
		if (cmp == 0)
			break;
		dir = (cmp > 0) ? 1 : 0;
		node = node->son[dir];
	}

	return node;
}

void gds_inline_rbtree_swap_nodes(gds_inline_rbtree_node_t *node1,
	gds_inline_rbtree_node_t *parent1, gds_inline_rbtree_node_t *node2,
	gds_inline_rbtree_node_t *parent2)
{
	gds_inline_rbtree_node_t tmp;

	if (parent1->son[0] == node1) parent1->son[0] = node2;
	else parent1->son[1] = node2;

	if (parent2->son[0] == node2) parent2->son[0] = node1;
	else parent2->son[1] = node1;

	tmp.red = node1->red;
	tmp.son[0] = node1->son[0];
	tmp.son[1] = node1->son[1];

	node1->red = node2->red;
	node1->son[0] = node2->son[0];
	node1->son[1] = node2->son[1];

	node2->red = tmp.red;
	node2->son[0] = tmp.son[0];
	node2->son[1] = tmp.son[1];
}

gds_inline_rbtree_node_t * gds_inline_rbtree_del(
	gds_inline_rbtree_node_t **root, const void *key,
	gds_rbt_cmp_with_key_cb rbt_cmp_with_key_cb, void *rbt_cmp_data)
{
	gds_inline_rbtree_node_t head;       /* False tree root */
	gds_inline_rbtree_node_t *q, *p, *g; /* Helpers */
	gds_inline_rbtree_node_t *f = NULL, *fp = NULL;  /* Found item and its parent */
	uint8_t dir = 1;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(*root);
	GDS_CHECK_ARG_NOT_NULL(rbt_cmp_with_key_cb);

	/* Set up helpers */
	q = &head;
	g = p = NULL;
	q->son[0] = NULL;
	q->son[1] = *root;

	/* Search and push a red down */
	while (q->son[dir] != NULL) {
		uint8_t last = dir;
		int32_t cmp;

		/* Update helpers */
		g = p, p = q;
		q = q->son[dir];
		cmp = rbt_cmp_with_key_cb(q, key, rbt_cmp_data);
		dir = (cmp > 0) ? 1 : 0;

		/* Save found node */
		if (cmp == 0) {
			f = q;
			fp = p;
		}

		if (gds_inline_rbtree_node_is_red(q)
		|| gds_inline_rbtree_node_is_red(q->son[dir]))
			continue;

		/* Push the red node down */
		if (gds_inline_rbtree_node_is_red(q->son[!dir])) {
			p = p->son[last] = gds_inline_rbtree_rotate(q, dir);
			if (q == f) fp = p;
		}
		else if (!gds_inline_rbtree_node_is_red(q->son[!dir])) {
			gds_inline_rbtree_node_t *s = p->son[!last];

			if (s == NULL)
				continue;

			if (!gds_inline_rbtree_node_is_red(s->son[!last])
			&& !gds_inline_rbtree_node_is_red(s->son[last])) {
				/* Color flip */
				p->red = false;
				s->red = true;
				q->red = true;
			} else {
				uint8_t dir2 = (g->son[1] == p) ? 1 : 0;

				if (gds_inline_rbtree_node_is_red(s->son[last])) {
					g->son[dir2] = gds_inline_rbtree_rotate_twice(p, last);
					if (f == p) fp = g->son[dir2];
				} else if (gds_inline_rbtree_node_is_red(s->son[!last])) {
					g->son[dir2] = gds_inline_rbtree_rotate(p, last);
					if (f == p) fp = g->son[dir2];
				}

				/* Ensure correct coloring */
				q->red = g->son[dir2]->red = true;
				g->son[dir2]->son[0]->red = false;
				g->son[dir2]->son[1]->red = false;
			}
		}
	}

	/* Replace and remove if found */
	if (f != NULL) {
		uint8_t dir1, dir2;

		gds_inline_rbtree_swap_nodes(f, fp, q, p);

		if (f == p) p = q;
		dir1 = (p->son[1] == f) ? 1 : 0;
		dir2 = (f->son[0] == NULL) ? 1 : 0;
		p->son[dir1] = f->son[dir2];
	}

	/* Update root and make it black */
	*root = head.son[1];
	if (*root != NULL)
		(*root)->red = false;

	return f;
}

typedef struct {
	gds_inline_rbtree_node_t *root;
	gds_slist_t *nodes;
	gds_iterator_t *slist_it;
} gds_inline_rbtree_iterator_data_t;

void gds_inline_rbtree_iterator_fill_list(gds_slist_t *nodes,
	gds_inline_rbtree_node_t *root)
{
	if (root != NULL) {
		gds_inline_rbtree_iterator_fill_list(nodes, root->son[0]);
		gds_slist_push(nodes, root);
		gds_inline_rbtree_iterator_fill_list(nodes, root->son[1]);
	}
}

int gds_inline_rbtree_iterator_reset(gds_inline_rbtree_iterator_data_t *data)
{
	gds_iterator_free(data->slist_it);
	gds_slist_free(data->nodes, NULL, NULL);

	data->nodes = gds_slist_new();
	gds_inline_rbtree_iterator_fill_list(data->nodes, data->root);
	data->slist_it = gds_slist_iterator_new(data->nodes);

	return 0;
}

int gds_inline_rbtree_iterator_step(gds_inline_rbtree_iterator_data_t *data)
{
	return gds_iterator_step(data->slist_it);
}

gds_inline_rbtree_node_t * gds_inline_rbtree_iterator_get(
	gds_inline_rbtree_iterator_data_t *data)
{
	return gds_iterator_get(data->slist_it);
}

const void * gds_inline_rbtree_iterator_getkey(
	gds_inline_rbtree_iterator_data_t *data)
{
	return gds_iterator_getkey(data->slist_it);
}

void gds_inline_rbtree_iterator_data_free(
	gds_inline_rbtree_iterator_data_t *data)
{
	gds_iterator_free(data->slist_it);
	gds_slist_free(data->nodes, NULL, NULL);
	free(data);
}

gds_iterator_t * gds_inline_rbtree_iterator_new(
	gds_inline_rbtree_node_t *root)
{
	gds_inline_rbtree_iterator_data_t *it_data;
	gds_iterator_t *it;

	GDS_CHECK_ARG_NOT_NULL(root);

	it_data = malloc(sizeof(gds_inline_rbtree_iterator_data_t));
	it_data->root = root;
	it_data->nodes = NULL;
	it_data->slist_it = NULL;

	it = gds_iterator_new(it_data,
		(gds_iterator_reset_cb) gds_inline_rbtree_iterator_reset,
		(gds_iterator_step_cb) gds_inline_rbtree_iterator_step,
		(gds_iterator_get_cb) gds_inline_rbtree_iterator_get,
		(gds_iterator_getkey_cb) gds_inline_rbtree_iterator_getkey,
		(gds_free_cb) gds_inline_rbtree_iterator_data_free);

	return it;
}
