#ifndef __SLList_h__
#define __SLList_h__

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

#endif /* Not __SLList_h__ */

