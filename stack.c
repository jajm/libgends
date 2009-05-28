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
	S->first = NULL;
	S->num_bytes = num_bytes;

	return S;
}


void Stack_push(Stack *S, void *data)
{
	stack_elt *s;

	s = malloc(sizeof(stack_elt));
	if(s == NULL){
		fprintf(stderr, "Erreur allocation mémoire\n");
		exit(-1);
	}
	if(data == NULL){
		fprintf(stderr, "Attention, tentative d'insertion d'élément NULL dans la pile\n");
		return;
	}

	s->data = malloc(S->num_bytes);
	if(s->data == NULL){
		fprintf(stderr, "Erreur allocation mémoire\n");
		exit(-1);
	}
	memcpy(s->data, data, S->num_bytes);

	/* Ajout du nouvel élément en haut de la pile */
	s->next = S->first;
	S->first = s;
}

void Stack_pop(Stack *S, void *dest)
{
	stack_elt *tmp;
	
	if(S->first == NULL){
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

