#ifndef __log_h__
#define __log_h__

#include <stdio.h>

/* Macros for writing to logs with a shorter name */
#define WHERESTR "%s:%d: "
#define WHEREARG __FILE__, __LINE__
#define Err(fmt) logger(Error, WHERESTR fmt "\n", WHEREARG)
#define Warn(fmt) logger(Warning, WHERESTR fmt "\n", WHEREARG)
#define Info(fmt) logger(Info, WHERESTR fmt "\n", WHEREARG)

/* Names of files which will be used for logging */
struct logfilenames{
	char *info;
	char *warn;
	char *err;
	char *all;
};

/* FILE structures corresponding to `logfilenames` */
struct logfiles{
	FILE *info;
	FILE *warn;
	FILE *err;
	FILE *all;
};

enum loglevel{
	Info,
	Warning,
	Error
};

/* Global Variables */
struct logfilenames def_logfn;
struct logfiles def_logf;

/* Logging functions */

/* Set default log filenames */
int set_def_logfilenames(const char *info, const char *warn, const char *err,
	const char *all);

int logger_init(void);
int logger_destroy(void);

int logger(enum loglevel level, const char *fmt, ...);

#endif /* Not __log_h__ */

