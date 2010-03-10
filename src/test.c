#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "types.h"
#include "error.h"
#include "generic.h"
#include "vw_sllist.h"

struct test_t{
	u32 i;
	double f;
	char *s;
};

int main()
{
	error_ptr error = NULL;
	generic_ptr gen;
	s8 d = 97;
	s8 *d_ptr = NULL;
	struct test_t t;
	struct test_t *t_ptr = NULL;
	vw_sllist head = NULL;
	struct vw_sllist_node *tmp;

	type_init();
	error_init(&error);

	type_reg("test_t", sizeof(struct test_t));
	
	t.i = 23;
	t.f = 9.876;
	t.s = malloc(10);
	t.s[0] = 'a'; t.s[1] = 'b'; t.s[9] = '\0';
	gen = generic("test_t", &t, &error);
	if(!gen){
		error_print(error);
		return error->errno;
	}
	vw_sllist_add(&head, 0, gen);
	vw_sllist_add(&head, 0, generic("s8", &d, NULL));
	tmp = head;
	while(tmp != NULL){
		if(strcmp(tmp->d->type_name, "s8") == 0){
			d_ptr = (s8 *)(tmp->d->data_ptr);
			printf("%c\n", *d_ptr);
		}else if(strcmp(tmp->d->type_name, "test_t") == 0){
			t_ptr = (struct test_t *)(tmp->d->data_ptr);
			printf("i = %d f = %f s = %s\n",
				t_ptr->i, t_ptr->f, t_ptr->s);
		}
		tmp = tmp->next;
	}
	
	return 0;
}

