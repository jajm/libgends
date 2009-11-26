#include <stdio.h>
#include <string.h>
#include "generic.h"
#include "l_list.h"

int main()
{
	struct l_list_node *head = NULL;
	int a, b, c;

	a = 1; b = 2; c = 3;

	llist_add(&head, -1, generic(sizeof(int), &a));
	llist_add(&head, -1, generic(sizeof(int), &b));
	llist_add(&head, -1, generic(sizeof(int), &c));
	
	llist_print(head);

	llist_del(&head, 1);
	llist_print(head);
	llist_del(&head, 1);
	llist_print(head);
	llist_del(&head, 1);
	llist_print(head);
	llist_del(&head, 1);
	llist_print(head);

	llist_free(&head);

	return 0;
}

