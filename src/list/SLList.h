#ifndef __SLList_h__
#define __SLList_h__

#include "err_code.h"
#include "llist.h"
#include "fw_sllist.h"
#include "vw_sllist.h"

typedef struct{
	union{
		fw_sllist fw;
		vw_sllist vw;
	} list;
	llist_type_t type;
} *SLList;

SLList SLList_new_fixed_width(fw_llist_width_t width);
SLList SLList_new_variable_width(void);

#endif /* Not __SLList_h__ */

