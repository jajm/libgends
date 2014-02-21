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

/*
 * Initialize structure
 */
void
gds_inline_slist_node_init(
	gds_inline_slist_node_t *node
);

/*
 * Set the next node
 *
 * Parameters
 *   node: The node to modify
 *   next: Node to set as the next node
 *
 * Returns:
 *   0 on success
 *   a negative value on failure
 */
int
gds_inline_slist_node_set_next(
	gds_inline_slist_node_t *node,
	gds_inline_slist_node_t *next
);

/*
 * Get the next node
 *
 * Parameters
 *   node: Pointer to node
 *
 * Returns:
 *   pointer to the next node.
 */
gds_inline_slist_node_t *
gds_inline_slist_node_get_next(
	gds_inline_slist_node_t *node
);

/*
 * Insert <list> immediately after <node>.
 *
 * Parameters:
 *   node: reference node
 *   list: a node in the list to insert
 *   newtail: if not NULL, and if <node> was the tail of the list, *newtail will
 *            contain the address of the new tail of the list
 *
 * Example:
 *   If we have 2 lists:
 *     N -> N1 -> N2
 *     P -> P1 -> P2
 *   calling gds_inline_slist_node_append(N, P, NULL) will result in:
 *     N -> P -> P1 -> P2 -> N1 -> N2
 *
 * Note: <list> should be the first node of its list and should not be already
 * in the same list than <node>, otherwise you may experience problems.
 *
 * Return:
 *   number of added nodes, ie. size of inserted list
 *   or a negative value in case of failure.
 */
int
gds_inline_slist_node_append(
	gds_inline_slist_node_t *node,
	gds_inline_slist_node_t *list,
	gds_inline_slist_node_t **newtail
);

/*
 * Insert <list> into <head> list.
 *
 * Parameters
 *   head: Head of list to modify.
 *   offset: Position where list will be inserted. 0 means beginning.
 *   list: Head of list to insert.
 *   newhead: If not NULL and if head of list change, this parameter will
 *            contain address of the new head.
 *   newtail: If not NULL and if tail of list change, this parameter will
 *            contain address of the new tail.
 *
 * Return:
 *   number of added nodes, ie. size of inserted list
 *   or a negative value in case of failure.
 */
int
gds_inline_slist_insert(
	gds_inline_slist_node_t *head,
	unsigned int offset,
	gds_inline_slist_node_t *list,
	gds_inline_slist_node_t **newhead,
	gds_inline_slist_node_t **newtail
);

/*
 * Remove nodes from list.
 *
 * Parameters:
 *   node: a node in the list
 *   offset: removal starting point
 *           if offset > 0, start removal <offset> nodes after <node>
 *           if offset = 0, start removal at <node>
 *   length: number of nodes to remove
 *   callback: function called on node before removal
 *             prototype is void callback(gds_inline_slist_node_t *, void *)
 *             first parameter is the node that will be removed
 *             second parameter is <callback_data>
 *   callback_data: data passed to <callback>
 *   newhead: if not NULL, and if head of list was just removed, address of new
 *            head is affected to *newhead
 *   newtail: if not NULL, and if tail of list was just removed, address of new
 *            tail is affected to *newtail
 *
 * Returns:
 *   Number of removed nodes.
 */
int
gds_inline_slist_remove(
	gds_inline_slist_node_t *head,
	unsigned int offset,
	unsigned int length,
	void *callback,
	void *callback_data,
	gds_inline_slist_node_t **newhead,
	gds_inline_slist_node_t **newtail
);

/*
 * Remove the tail of the list.
 *
 * Parameters:
 *   head: Head of the list. Updated if head had changed.
 *   callback: Function called on node before removal. Prototype is
 *             void callback(gds_inline_slist_node_t *, void *)
 *   callback_data: Data passed to callback as 2nd parameter.
 *   newhead: if not NULL, and if head of list was just removed, address of new
 *            head is affected to *newhead
 *   newtail: if not NULL, and if tail of list was just removed, address of new
 *            tail is affected to *newtail
 */
int gds_inline_slist_remove_tail(
	gds_inline_slist_node_t *head,
	void *callback,
	void *callback_data,
	gds_inline_slist_node_t **newhead,
	gds_inline_slist_node_t **newtail
);

/*
 * Remove nodes from list and insert other nodes in place.
 *
 * Parameters:
 *   head: a node in the list
 *   offset: removal starting point
 *           if offset > 0, start removal <offset> nodes after <node>
 *           if offset = 0, start removal at <node>
 *   length: number of nodes to remove
 *   callback: function called on node before removal
 *             prototype is void callback(gds_inline_slist_node_t *, void *)
 *             first parameter is the node that will be removed
 *             second parameter is <callback_data>
 *   callback_data: data passed to <callback>
 *   list: A node in the list to insert in place.
 *   newhead: if not NULL, and if head of list was just removed, address of new
 *            head is affected to *newhead
 *   newtail: if not NULL, and if tail of list was just removed, address of new
 *            tail is affected to *newtail
 *
 * Returns:
 *   Number of added nodes.
 *   Can be negative if number of removed nodes is greater than the number of
 *   added nodes.
 */
int
gds_inline_slist_splice(
	gds_inline_slist_node_t *head,
	unsigned int offset,
	unsigned int length,
	void *callback,
	void *callback_data,
	gds_inline_slist_node_t *list,
	gds_inline_slist_node_t **newhead,
	gds_inline_slist_node_t **newtail
);

/*
 * Get a node in list from its position.
 *
 * Parameters:
 *   node: A node in the list.
 *   offset: Offset from <node>.
 *
 * Returns:
 *   Given N the position of <node>, it returns the node at position
 *   N + <offset>.
 *   If N + <offset> is out of bound, it returns NULL
 */
gds_inline_slist_node_t *
gds_inline_slist_get(
	gds_inline_slist_node_t *node,
	unsigned int offset
);

/*
 * Get the last node of list.
 *
 * Parameters:
 *   node: A node in the list
 *
 * Returns:
 *   The last node of the list.
 */
gds_inline_slist_node_t *
gds_inline_slist_get_tail(
	gds_inline_slist_node_t *node
);

/*
 * Get the list size
 *
 * Parameters:
 *   head: Head of list
 *
 * Returns:
 *   The number of elements in the list
 */
int
gds_inline_slist_size(
	gds_inline_slist_node_t *head
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

