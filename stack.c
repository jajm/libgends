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


int Stack_push(Stack *S, void *data)
{
	struct l_list_node *ll_node;

	ll_node = malloc(sizeof(struct l_list_node));
	if(ll_node == NULL){
		fprintf(stderr, "Erreur allocation mémoire\n");
		return -1;
	}
	if(data == NULL){
		fprintf(stderr, "Attention, tentative d'insertion d'élément NULL"
			"dans la pile\n");
		return -1;
	}
	
	ll_node->data = malloc(S->num_bytes);
	if(ll_node->data == NULL){
		fprintf(stderr, "Erreur allocation mémoire\n");
		exit(-1);
	}

	/* Data copy */
	memcpy(ll_node->data, data, S->num_bytes);

	/* Push the newly created stack element to the top of the stack */
	/* The top of the stack is the head of the list */
	ll_node->next = S->head;
	S->head = ll_node;

	return 0;
}

int Stack_pop(Stack *S, void *dest)
{
	struct l_list_node *ll_node;
	
	if(S->head == NULL){
		printf("Pile vide\n");
		return -1;
	}
	if(dest == NULL){
		printf("dest == NULL\n");
		return -1;
	}

	/* Data copy */
	memcpy(dest, S->head->data, S->num_bytes);
	
	/* Pop (remove) the copied data from stack */
	ll_node = S->head;
	S->head = S->head->next;
 	free(ll_node->data);
	free(ll_node);

	return 0;
}

