#include <stdlib.h>
#include <libexception/exception.h>
#include "log.h"
#include "malloc.h"

#define log_and_throw(exception, message, ...) { \
	gds_log_fatal(message, ##__VA_ARGS__); \
	throw(exception, message, ##__VA_ARGS__); \
}

void * gds_malloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (ptr == NULL) {
		log_and_throw(NotEnoughMemoryException,
			"malloc failed to allocate %d bytes", size);
	}

	return ptr;
}

void * gds_calloc(size_t nmemb, size_t size)
{
	void *ptr;

	ptr = calloc(nmemb, size);
	if (ptr == NULL) {
		log_and_throw(NotEnoughMemoryException,
			"calloc failed to allocate %d elements of %d bytes",
			nmemb, size);
	}

	return ptr;
}

void * gds_realloc(void *ptr, size_t size)
{
	ptr = realloc(ptr, size);
	if (ptr == NULL) {
		log_and_throw(NotEnoughMemoryException,
			"realloc failed to allocate %d bytes", size);
	}

	return ptr;
}
