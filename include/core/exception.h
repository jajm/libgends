#ifndef gds_exception_h_included
#define gds_exception_h_included

#include <libexception/exception.h>
#include "log.h"

#define GDS_THROW(exception, message, ...) \
	{ \
		GDS_LOG_ERROR(message, ##__VA_ARGS__); \
		throw(exception, message, ##__VA_ARGS__); \
	}

#define GDS_THROW_ALLOC_ERROR(size) \
	GDS_THROW(NotEnoughMemoryException, "failed to allocate %d bytes", size)

#define GDS_THROW_EMPTY_LIST() \
	GDS_THROW(EmptyListException, "list is empty")

#endif /* Not gds_exception_h_included */

