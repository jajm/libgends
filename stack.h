#ifndef __stack_h__
#define __stack_h__

typedef struct _stack_elt{
	void *data;
	struct _stack_elt *next;
} stack_elt;

typedef struct{
	stack_elt *first;
	unsigned int num_bytes;
} Stack;

Stack * Stack_new(unsigned int num_bytes);
void Stack_push(Stack *S, void *data);
void Stack_pop(Stack *S, void *dest);

#endif /* __stack_h__ */

