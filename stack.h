/*******************************************************************************
 * Name		: stack.h
 * Object	: stack management functions
 *******************************************************************************
 * Author	: Julian Maurice
 * License	: Public Domain
 ******************************************************************************/

#ifndef __stack_h__
#define __stack_h__

#include "l_list.h"

typedef struct{
	struct l_list_node *head;
	unsigned int num_bytes;
} Stack;

Stack * Stack_new(unsigned int num_bytes);
int Stack_push(Stack *S, void *data);
int Stack_pop(Stack *S, void *dest);

#endif /* __stack_h__ */
