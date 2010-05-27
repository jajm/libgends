#ifndef stack_h_included
#define stack_h_included

#include "slist.h"

typedef slist_t stack_t;

#define stack_new(type_name) slist_new(type_name)
#define stack_push(stack, data) slist_add_first(stack, data)
#define stack_pop(stack) slist_pop_first(stack)
#define stack_free(stack) slist_free(stack)
#define stack_destroy(stack) slist_destroy(stack)

#endif /* Not stack_h_included */

