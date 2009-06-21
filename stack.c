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


void Stack_push(Stack *S, void *data)
{
	l_list_node *ll_node;

	ll_node = malloc(sizeof(struct l_list_node));
	if(ll_node == NULL){
		fprintf(stderr, "Erreur allocation mémoire\n");
		exit(-1);
	}
	if(data == NULL){
		fprintf(stderr, "Attention, tentative d'insertion d'élément NULL"
			"dans la pile\n");
		return;
	}

	ll_node->data = malloc(S->num_bytes);
	if(ll_node->data == NULL){
		fprintf(stderr, "Erreur allocation mémoire\n");
		exit(-1);
	}
	memcpy(ll_node->data, data, S->num_bytes);

	/* Ajout du nouvel élément en haut de la pile */
	ll_node->next = S->head;
	S->head = ll_node;
}

void Stack_pop(Stack *S, void *dest)
{
	l_list_node *ll_node;
	
	if(S->head == NULL){
		printf("Pile vide\n");
		return;
	}
	memcpy(dest, S->first->data, S->num_bytes);
	
	/* Suppression du premier élément de la pile */
	tmp = S->first;
	S->first = S->first->next;
 	free(tmp->data);
	free(tmp);
}

