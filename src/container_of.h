#ifndef gds_container_of_h_included
#define gds_container_of_h_included

#include <stddef.h>

#define container_of(ptr, type, member) ({                   \
	const __typeof__( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - offsetof(type,member) );  \
})

#endif /* ! gds_container_of_h_included */
