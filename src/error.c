#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "error.h"

void error_init(error_ptr *err)
{
	struct error_t *tmp;

	if(err != NULL){
		tmp = *err;
		if(tmp == NULL){
			tmp = malloc(sizeof(struct error_t));
			if(tmp){
				tmp->errno = 0;
				tmp->errmsg = NULL;
				tmp->file = NULL;
				tmp->function = NULL;
				tmp->line = 0;
			}
			*err = tmp;
		}else{
			tmp->errno = 0;
			tmp->errmsg = NULL;
			tmp->file = NULL;
			tmp->function = NULL;
			tmp->line = 0;
		}
	}
}

void _error_set(error_ptr *err, s32 errno, const char *file,
	const char *function, u32 line, const char *errmsg, ...)
{
	size_t len;
	struct error_t *tmp;
	char buffer[1024];
	va_list vl;

	if(err != NULL){
		if(*err == NULL) error_init(err);
		tmp = *err;
		tmp->errno = errno;
		if(file != NULL){
			len = strlen(file);
			tmp->file = realloc(tmp->file, len+1);
			if(tmp->file != NULL){
				strncpy(tmp->file, file, len+1);
			}
		}else{
			free(tmp->file);
			tmp->file = NULL;
		}
		if(function != NULL){
			len = strlen(function);
			tmp->function = realloc(tmp->function, len+1);
			if(tmp->function != NULL){
				strncpy(tmp->function, function, len+1);
			}
		}else{
			free(tmp->function);
			tmp->function = NULL;
		}
		tmp->line = line;
		if(errmsg != NULL){
			va_start(vl, errmsg);
			len = vsprintf(buffer, errmsg, vl);
			va_end(vl);
			tmp->errmsg = realloc(tmp->errmsg, len+1);
			if(tmp->errmsg != NULL){
				strncpy(tmp->errmsg, buffer, len+1);
			}
		}else{
			free(tmp->errmsg);
			tmp->errmsg = NULL;
		}
	}
}

void error_print(const error_ptr err)
{
	if(err != NULL){
		printf("Error (%d) ", err->errno);
		if(err->function)
			printf("in %s ", err->function);
		if(err->file)
			printf("at %s:%d", err->file, err->line);
		printf(": ");
		if(err->errmsg)
			printf("%s\n", err->errmsg);
		else
			printf("No error message specified\n");
	}
}

void error_free(error_ptr *err)
{
	if(err != NULL && *err != NULL){
		free((*err)->errmsg);
		free((*err)->file);
		free((*err)->function);
		free(*err);
	}
	if(err != NULL) *err = NULL;
}

