#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

void error_init(struct error_t *err)
{
	if(err != NULL){
		err->errno = 0;
		err->errmsg = NULL;
		err->file = NULL;
		err->function = NULL;
		err->line = 0;
	}
}

void _error_set(struct error_t *err, s32 errno, const char *errmsg,
	const char *file, const char *function, u32 line)
{
	size_t len;

	if(err != NULL){
		err->errno = errno;
		if(errmsg != NULL){
			len = strlen(errmsg);
			err->errmsg = realloc(err->errmsg, len+1);
			if(err->errmsg != NULL){
				strncpy(err->errmsg, errmsg, len+1);
			}
		}else{
			free(err->errmsg);
			err->errmsg = NULL;
		}
		if(file != NULL){
			len = strlen(file);
			err->file = realloc(err->file, len+1);
			if(err->file != NULL){
				strncpy(err->file, file, len+1);
			}
		}else{
			free(err->file);
			err->file = NULL;
		}
		if(function != NULL){
			len = strlen(function);
			err->function = realloc(err->function, len+1);
			if(err->function != NULL){
				strncpy(err->function, function, len+1);
			}
		}else{
			free(err->function);
			err->function = NULL;
		}
		err->line = line;
	}
}

void error_print(struct error_t *err)
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

void error_free(struct error_t *err)
{
	if(err != NULL){
		free(err->errmsg);
		free(err->file);
		free(err->function);
		free(err);
	}
}
