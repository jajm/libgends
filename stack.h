#ifndef __stack_h__
#define __stack_h__

#include "l_list.h"

typedef struct{
	struct l_list_elt *head;
	unsigned int num_bytes;
} Stack;

Stack * Stack_new(unsigned int num_bytes);
void Stack_push(Stack *S, void *data);
void Stack_pop(Stack *S, void *dest);

#endif /* __stack_h__ */
