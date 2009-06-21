#include "log.h"

int main()
{
	set_def_logfilenames("info.log", "warn.log", "err.log", "all.log");
	logger_init();

	Err("aaaaaaaaaaaaah");
	Warn("oooooooooooooh");
	Info("eeeeeeeeeeeeeh");

	logger_destroy();

	return 0;
}

