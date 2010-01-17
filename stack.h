/*******************************************************************************
 * Name		: stack.h
 * Object	: stack management functions
 *******************************************************************************
 * Author	: Julian Maurice
 * License	: Public Domain
 ******************************************************************************/

#ifndef __stack_h__
#define __stack_h__

#include "types.h"
#include "err_code.h"
#include "llist.h"

typedef  stack_type_t;

typedef struct{
	enum{
		FIXED_WIDTH,
		VARIABLE_WIDTH
	} type;
	union{
		vw_llist vw;	/* Variable-width list */
		llist fw;	/* Fixed-width list */
	} list;
} Stack;

/* Create a new stack.
 * num_bytes	: size of a stack element
 * Returns the adress of the newly created Stack
 */
Stack * Stack_new(u32 width);
Stack * Stack_new_vw(void);

/* Push data into stack
 * S	: Stack adress
 * data	: data adress
 * Returns a negative value if an error occurs, 0 otherwise.
 */
int Stack_push(Stack *S, void *data);

/* Pop data from stack
 * S	: Stack adress
 * dest	: adress of an allocated memory block where data will be copied
 * Returns a negative value if an error occurs, 0 otherwise.
 */
int Stack_pop(Stack *S, void *dest);

#endif /* __stack_h__ */
