#ifndef queue_h_included
#define queue_h_included

#include "slist.h"

typedef slist_t queue_t;

#define queue_new(type_name) slist_new(type_name)
#define queue_push(queue, data) slist_add_last(queue, data)
#define queue_pop(queue) slist_pop_first(queue)
#define queue_free(queue) slist_free(queue)
#define queue_destroy(queue) slist_destroy(queue)

#endif /* Not queue_h_included */

