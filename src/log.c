#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "log.h"

int set_def_logfilenames(const char *info, const char *warn, const char *err,
	const char *all)
{
	def_logfn.info = malloc((strlen(info)+1)*sizeof(char));
	strcpy(def_logfn.info, info);
	def_logfn.warn = malloc((strlen(warn)+1)*sizeof(char));
	strcpy(def_logfn.warn, warn);
	def_logfn.err = malloc((strlen(err)+1)*sizeof(char));
	strcpy(def_logfn.err, err);
	def_logfn.all = malloc((strlen(all)+1)*sizeof(char));
	strcpy(def_logfn.all, all);
	
	return 0;
}

int logger_init(void)
{	
	def_logf.info = fopen(def_logfn.info, "w");
	def_logf.warn = fopen(def_logfn.warn, "w");
	def_logf.err = fopen(def_logfn.err, "w");
	def_logf.all = fopen(def_logfn.all, "w");

	return 0;
}

int logger_destroy(void)
{
	fclose(def_logf.info);
	fclose(def_logf.warn);
	fclose(def_logf.err);
	fclose(def_logf.all);

	return 0;
}

int logger(enum loglevel level, const char *fmt, ...)
{
	FILE *screen, *global_file, *specific_file;
	va_list vl;

	switch(level){
	case Info:
		screen = stdout;
		specific_file = def_logf.info;
		break;
	case Warning:
		screen = stderr;
		specific_file = def_logf.warn;
		break;
	case Error:
		screen = stderr;
		specific_file = def_logf.err;
		break;
	default:
		screen = stdout;
		specific_file = def_logf.info;
		break;
	}
	global_file = def_logf.all;

	va_start(vl, fmt);
	vfprintf(screen, fmt, vl);
	va_end(vl);

	va_start(vl, fmt);
	vfprintf(global_file, fmt, vl);
	va_end(vl);
	
	va_start(vl, fmt);
	vfprintf(specific_file, fmt, vl);
	va_end(vl);
#ifdef DEBUG
	fflush(screen);
	fflush(global_file);
	fflush(specific_file);
#endif
	return 0;
}

