#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

Stack * Stack_new(unsigned int num_bytes)
{
	Stack *S;

	S = malloc(sizeof(Stack));
	if(S == NULL){
		fprintf(stderr, "Erreur allocation mémoire\n");
		exit(-1);
	}
	S->head = NULL;
	S->num_bytes = num_bytes;

	return S;
}


err_code Stack_push(Stack *S, void *data)
{
	return llist_add(&(S->head), 0, generic(S->num_bytes, data));
}

err_code Stack_pop(Stack *S, void *dest)
{
	generic_t g;
	
	if(S->head == NULL || dest == NULL)
		return PARAM_VALUE_ERROR;

	/* Data copy */
	g = llist_get(S->head, 0);
	memmove(dest, generic_data(g), S->num_bytes);
	
	/* Pop (remove) the copied data from stack */
	llist_del(&(S->head), 0);

	return 0;
}

