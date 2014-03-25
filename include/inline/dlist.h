/*
 * Copyright 2013-2014 Julian Maurice
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

#ifndef gds_inline_dlist_h_included
#define gds_inline_dlist_h_included

typedef struct gds_inline_dlist_node_s {
	struct gds_inline_dlist_node_s *prev;
	struct gds_inline_dlist_node_s *next;
} gds_inline_dlist_node_t;

/* Initialize structure
 *
 * Parameters
 *   node : Node to initialize.
 *
 * Returns
 *   0 on success
 *   a negative value on failure
 */
int
gds_inline_dlist_node_init(
	gds_inline_dlist_node_t *node
);

/* Set the previous node
 *
 * Parameters
 *   node: The node to modify
 *   prev: Node to set as the previous node
 *
 * Returns:
 *   0 on success
 *   a negative value on failure
 */
int
gds_inline_dlist_node_set_prev(
	gds_inline_dlist_node_t *node,
	gds_inline_dlist_node_t *prev
);

/* Get the previous node
 *
 * Parameters
 *   node: Pointer to node
 *
 * Returns:
 *   pointer to the previous node
 */
gds_inline_dlist_node_t *
gds_inline_dlist_node_get_prev(
	gds_inline_dlist_node_t *node
);

/* Set the next node
 *
 * Parameters
 *   node: The node to modify
 *   next: Node to set as the next node
 *
 * Returns
 *   0 on success
 *   a negative value on failure
 */
int
gds_inline_dlist_node_set_next(
	gds_inline_dlist_node_t *node,
	gds_inline_dlist_node_t *next
);

/* Get the next node
 *
 * Parameters
 *   node: Pointer to node
 *
 * Returns
 *   pointer to the next node
 */
gds_inline_dlist_node_t *
gds_inline_dlist_node_get_next(
	gds_inline_dlist_node_t *node
);

/* Insert <list> immediately before <node>.
 *
 * Parameters
 *   node    : reference node
 *   list    : a node in the list to insert
 *   newhead : if not NULL, and if <node> was the head of the list, *newhead
 *             will contain the address of the new head of the list
 *
 * Example
 *   If we have 2 lists:
 *     N <-> N1 <-> N2
 *     P <-> P1 <-> P2
 *   calling gds_inline_dlist_node_prepend(N1, P1) will result in:
 *     N <-> P <-> P1 <-> P2 <-> N1 <-> N2
 *
 * Note: <list> should not be already in the same list than <node>, otherwise
 * you may experience problems.
 *
 * Returns
 *   number of added nodes, ie. size of inserted list
 *   0 on failure
 */
unsigned int
gds_inline_dlist_node_prepend(
	gds_inline_dlist_node_t *node,
	gds_inline_dlist_node_t *list,
	gds_inline_dlist_node_t **newhead
);

/* Insert <list> immediately after <node>.
 *
 * Parameters
 *   node    : reference node
 *   list    : a node in the list to insert
 *   newtail : if not NULL, and if <node> was the tail of the list, *newtail
 *             will contain the address of the new tail of the list
 *
 * Example
 *   If we have 2 lists:
 *     N <-> N1 <-> N2
 *     P <-> P1 <-> P2
 *   calling gds_inline_dlist_node_append(N1, P1) will result in:
 *     N <-> N1 <-> P <-> P1 <-> P2 <-> N2
 *
 * Note: <list> should not be already in the same list than <node>, otherwise
 * you may experience problems.
 *
 * Returns
 *   number of added nodes, ie. size of inserted list
 *   0 on failure
 */
unsigned int
gds_inline_dlist_node_append(
	gds_inline_dlist_node_t *node,
	gds_inline_dlist_node_t *list,
	gds_inline_dlist_node_t **newtail
);

/* Insert nodes in list.
 *
 * Parameters
 *   node    : A node in the list.
 *   offset  : Where to insert the nodes in the list. Can be negative.
 *   list    : A node in the list to insert.
 *   newhead : if not NULL, and if head of list was just changed, address of new
 *             head is affected to *newhead
 *   newtail : if not NULL, and if tail of list was just changed, address of new
 *             tail is affected to *newtail
 *
 * Returns:
 *   number of added nodes, ie. size of inserted list
 *   0 on failure
 */
unsigned int gds_inline_dlist_insert(
	gds_inline_dlist_node_t *node,
	int offset,
	gds_inline_dlist_node_t *list,
	gds_inline_dlist_node_t **newhead,
	gds_inline_dlist_node_t **newtail
);

/* Remove nodes from list.
 *
 * Parameters
 *   node          : a node in the list
 *   offset        : removal starting point
 *                   if offset > 0, start removal <offset> nodes after <node>
 *                   if offset < 0, start removal <offset> nodes before <node>
 *                   if offset = 0, start removal at <node>
 *   length        : number of nodes to remove
 *                   if length > 0, remove in direction of tail
 *                   if length < 0, remove in direction of head
 *   callback      : function called on node before removal
 *                   prototype: void callback(gds_inline_dlist_node_t *node)
 *                   - node: the node that will be removed
 *   newhead       : if not NULL, and if head of list was just removed, address
 *                   of new head is affected to *newhead
 *   newtail       : if not NULL, and if tail of list was just removed, address
 *                   of new tail is affected to *newtail
 *
 * Returns
 *   Number of removed nodes.
 *   0 on failure
 */
unsigned int
gds_inline_dlist_remove(
	gds_inline_dlist_node_t *node,
	int offset,
	int length,
	void *callback,
	gds_inline_dlist_node_t **newhead,
	gds_inline_dlist_node_t **newtail
);

/* Remove nodes from list and insert other nodes in place.
 *
 * Parameters
 *   node          : a node in the list
 *   offset        : removal starting point
 *                   if offset > 0, start removal <offset> nodes after <node>
 *                   if offset < 0, start removal <offset> nodes before <node>
 *                   if offset = 0, start removal at <node>
 *   length        : number of nodes to remove
 *                   if length > 0, remove in direction of tail
 *                   if length < 0, remove in direction of head
 *   callback      : function called on node before removal
 *                   prototype: void callback(gds_inline_dlist_node_t *node)
 *                   - node: first parameter is the node that will be removed
 *   list          : A node in the list to insert in place.
 *   newhead       : if not NULL, and if head of list was just removed, address
 *                   of new head is affected to *newhead
 *   newtail       : if not NULL, and if tail of list was just removed, address
 *                   of new tail is affected to *newtail
 *
 * Returns:
 *   Number of added nodes.
 *   Can be negative if number of removed nodes is greater than the number of
 *   added nodes.
 */
int
gds_inline_dlist_splice(
	gds_inline_dlist_node_t *node,
	int offset,
	int length,
	void *callback,
	gds_inline_dlist_node_t *list,
	gds_inline_dlist_node_t **newhead,
	gds_inline_dlist_node_t **newtail
);

/* Get a node in list from its position.
 *
 * Parameters
 *   node   : A node in the list.
 *   offset : Offset from <node>. Can be negative.
 *
 * Returns:
 *   Given N the position of <node>, it returns the node at position
 *   N + <offset>.
 *   If N + <offset> is out of bound, it returns NULL
 */
gds_inline_dlist_node_t *
gds_inline_dlist_get(
	gds_inline_dlist_node_t *node,
	int offset
);

/* Get the first node of list.
 *
 * Parameters
 *   node : A node in the list
 *
 * Returns
 *   The first node of the list.
 */
gds_inline_dlist_node_t *
gds_inline_dlist_get_head(
	gds_inline_dlist_node_t *node
);

/* Get the last node of list.
 *
 * Parameters
 *   node : A node in the list
 *
 * Returns
 *   The last node of the list.
 */
gds_inline_dlist_node_t *
gds_inline_dlist_get_tail(
	gds_inline_dlist_node_t *node
);

/* Get the list size
 *
 * Parameters
 *   head : Head of list
 *
 * Returns
 *   The number of elements in the list
 */
unsigned int
gds_inline_dlist_size(
	gds_inline_dlist_node_t *head
);

/* Apply a function to each node in list.
 *
 * Parameters:
 *   head          : Head of the list.
 *   callback      : Function called on each node. Parameters are:
 *                   - (gds_inline_dlist_node_t *) current node
 *
 * Returns
 *   0 on success
 *   a negative value on failure
 */
int
gds_inline_dlist_map(
	gds_inline_dlist_node_t *head,
	void *callback
);

#endif /* ! gds_inline_dlist_h_included */

