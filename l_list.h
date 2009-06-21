/*******************************************************************************
 * Name		: l_list.h
 * Object	: Simple linked-lists structure definition
 *******************************************************************************
 * Author	: Julian Maurice
 * License	: Public Domain
 ******************************************************************************/

#ifndef __l_list_h__
#define __l_list_h__

/* Linked-list node */
struct l_list_node{
	void *data;
	struct l_list_node *next;
};

#endif /* Not __l_list_h__ */
