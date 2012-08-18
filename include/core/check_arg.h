#ifndef check_arg_h_included
#define check_arg_h_included

#include "exception.h"

#define GDS_CHECK_ARG_NOT_NULL(arg) \
	if ((arg) == NULL) \
		GDS_THROW(BadArgumentException, "argument '%s' is NULL", #arg)

#endif /* Not check_arg_h_included */
