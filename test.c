#include <stdio.h>
#include <string.h>
#include "generic.h"
#include "l_list.h"

int main()
{
	struct l_list_node *head = NULL;
	generic_t g;
	int a, b;

	a = 1; b = 2;

	llist_add(&head, -1, generic(sizeof(int), &a));
	llist_add(&head, 0, generic(sizeof(int), &b));
	
	g = generic(sizeof(int), &a);
	printf("a est a la position %d\n", llist_chk(head, g));
	generic_free(&g);
	llist_free(&head);

	return 0;
}

