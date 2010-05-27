#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "error.h"

typedef struct {
	char *msg;
	char *file;
	char *function;
	u32 line;

	char *string;	/* Error in 'function' at 'file':'line': 'msg' */
} error_t;

error_t g_error;

void Error_init(void)
{
	g_error.msg = NULL;
	g_error.file = NULL;
	g_error.function = NULL;
	g_error.line = 0;
	g_error.string = NULL;
}

void Error_at(const char *file, u32 line, const char *function,
	 const char *msg, ...)
{
	size_t len;
	char buffer[1024];
	va_list vl;

	if(file != NULL){
		len = strlen(file);
		g_error.file = realloc(g_error.file, len+1);
		if(g_error.file != NULL){
			strncpy(g_error.file, file, len+1);
		}
	}else{
		free(g_error.file);
		g_error.file = NULL;
	}
	g_error.line = line;
	if(function != NULL){
		len = strlen(function);
		g_error.function = realloc(g_error.function, len+1);
		if(g_error.function != NULL){
			strncpy(g_error.function, function, len+1);
		}
	}else{
		free(g_error.function);
		g_error.function = NULL;
	}
	if(msg != NULL){
		va_start(vl, msg);
		len = vsprintf(buffer, msg, vl);
		va_end(vl);
		g_error.msg = realloc(g_error.msg, len+1);
		if(g_error.msg != NULL){
			strncpy(g_error.msg, buffer, len+1);
		}
	}else{
		free(g_error.msg);
		g_error.msg = NULL;
	}
	len = sprintf(buffer, "Error");
	if(g_error.function)
		len += sprintf(buffer+len, " in %s", g_error.function);
	if(g_error.file)
		len += sprintf(buffer+len, " at %s:%d", g_error.file,
			g_error.line);
	len += sprintf(buffer+len, ": ");
	if(g_error.msg)
		len += sprintf(buffer+len, "%s", g_error.msg);
	else
		len += sprintf(buffer+len, "No error message specified");
	g_error.string = realloc(g_error.string, len+1);
	if(g_error.string)
		strncpy(g_error.string, buffer, len+1);
}

void pError(void)
{
	fflush(stdout);
	fprintf(stderr, "%s\n", g_error.string);
}

char * Error_msg(void)
{
	return g_error.msg;
}

char * Error_file(void)
{
	return g_error.file;
}

char * Error_function(void)
{
	return g_error.function;
}

u32 Error_line(void)
{
	return g_error.line;
}

char *Error_string(void)
{
	return g_error.string;
}

