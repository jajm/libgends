#ifndef __logger_h__
#define __logger_h__

#include <stdio.h>

/* Macros for writing to logs with a shorter name */
#define WHERESTR "%s:%d: "
#define WHEREARG __FILE__, __LINE__
#define Err(fmt) logger(Error, WHERESTR fmt "\n", WHEREARG)
#define Warn(fmt) logger(Warning, WHERESTR fmt "\n", WHEREARG)
#define Info(fmt) logger(Info, WHERESTR fmt "\n", WHEREARG)

enum loglevel{
	Info,
	Warning,
	Error
};

/* Set default log filenames */
int set_def_logfilenames(const char *info, const char *warn, const char *err,
	const char *all);

int logger_init(void);
int logger_destroy(void);

int logger(enum loglevel level, const char *fmt, ...);

#endif /* Not __logger_h__ */

