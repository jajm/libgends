#include <stdio.h>
#include <string.h>
#include "generic.h"
#include "llist.h"

int main()
{
	llist head = NULL;
	int a, b, c;

	a = 1; b = 2; c = 3;

	head = llist_new(sizeof(int));

	llist_add(head, -1, &a);
	llist_add(head, -1, &b);
	llist_add(head, -1, &c);
	
	llist_print(head);

	llist_del(head, 1);
	llist_print(head);
	llist_del(head, 1);
	llist_print(head);
	llist_del(head, 1);
	llist_print(head);
	llist_del(head, 1);
	llist_print(head);

	llist_free(&head);

	return 0;
}

