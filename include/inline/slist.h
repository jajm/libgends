/*
 * Copyright 2013 Julian Maurice
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

#ifndef gds_inline_slist_h_included
#define gds_inline_slist_h_included

typedef struct gds_inline_slist_node_s {
	struct gds_inline_slist_node_s *next;
} gds_inline_slist_node_t;

/**
 * Initialize structure
 */
void
gds_inline_slist_node_init(
	gds_inline_slist_node_t *node
);

/**
 * Returns the node immediately after <node>
 */
gds_inline_slist_node_t *
gds_inline_slist_node_get_next(
	gds_inline_slist_node_t *node
);

int
gds_inline_slist_node_set_next(
	gds_inline_slist_node_t *node,
	gds_inline_slist_node_t *head
);

/**
 * Insert <head> immediately after <node>.
 *
 * Nodes following <head> are inserted too.
 * So if we have 2 lists:
 *   N -> N1 -> N2
 *   P -> P1 -> P2
 * calling gds_inline_slist_node_append_list(N, P, NULL) will result in:
 *   N -> P -> P1 -> P2 -> N1 -> N2
 *
 * Note: <head> should be the first node of its list and should not be already
 * in the same list than <node>, otherwise you may experience problems.
 *
 * Return:
 *   number of added nodes, ie. size of inserted list
 *   or a negative value in case of failure.
 */
int
gds_inline_slist_node_append_list(
	gds_inline_slist_node_t *node,
	gds_inline_slist_node_t *head,
	gds_inline_slist_node_t **newtail
);

/**
 * Insert <head2> list into <head1> list.
 *
 * Parameters
 *   head1   : Head of list to modify.
 *   offset  : Position where list will be inserted. 0 means beginning.
 *   head2   : Head of list to insert.
 *   newhead : If not NULL and if head of list change, this parameter will
 *             contain address of the new head.
 *   newtail : If not NULL and if tail of list change, this parameter will
 *             contain address of the new tail.
 *
 * Return:
 *   number of added nodes, ie. size of inserted list
 *   or a negative value in case of failure.
 */
int
gds_inline_slist_insert(
	gds_inline_slist_node_t **head1,
	unsigned int offset,
	gds_inline_slist_node_t *head2,
	gds_inline_slist_node_t **newtail
);

/**
 * Remove <length> nodes, starting <offset> nodes after <node> and call
 * <callback> for each removed node with <callback_data> as 2nd parameter.
 * Prototype for callback is callback(gds_inline_slist_node_t *, void *)
 */
int
gds_inline_slist_remove(
	gds_inline_slist_node_t **head,
	unsigned int offset,
	unsigned int length,
	void *callback,
	void *callback_data,
	gds_inline_slist_node_t **newtail
);

/**
 * Remove the last node of the list.
 *
 * Parameters:
 *   head          : Head of the list. Updated if head had changed.
 *   callback      : Function called on node before removal. Prototype is
 *                   void callback(gds_inline_slist_node_t *, void *)
 *   callback_data : Data passed to callback as 2nd parameter.
 *   newtail       : If not NULL, new last node address is affected to *newlast.
 */
int gds_inline_slist_remove_last(
	gds_inline_slist_node_t **head,
	void *callback,
	void *callback_data,
	gds_inline_slist_node_t **newtail
);

/**
 * Returns the <offset>-th node after <node>.
 * If <offset> is 0, returns <node>
 */
gds_inline_slist_node_t *
gds_inline_slist_get(
	gds_inline_slist_node_t *head,
	unsigned int offset
);

/**
 * Apply a function to each node in list.
 *
 * Parameters
 *   head          : Head of the list.
 *   callback      : Function called on each node. Parameters are:
 *                   - (gds_inline_slist_node_t *) current node
 *                   - (unsigned int) node offset
 *                   - (void *) callback_data
 *   callback_data : Data to pass to callback as 3rd parameter.
 */
void
gds_inline_slist_map(
	gds_inline_slist_node_t *head,
	void *callback,
	void *callback_data
);

#endif /* ! gds_inline_slist_h_included */

