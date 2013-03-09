#include "log.h"

int main(void)
{
	GDS_LOG_FATAL("fatal message");
	GDS_LOG_ERROR("error message");
	GDS_LOG_WARNING("warning message");
	GDS_LOG_INFO("info message");
	GDS_LOG_DEBUG("debug message");

	GDS_LOG_INFO("%d+%d = %d", 3, 4, 3+4);

	char *verbosity = getenv("LIBGENDS_VERBOSITY");
	if (verbosity == NULL) {
		printf("Environment variable LIBGENDS_VERBOSITY was not set. "
			"You probably saw nothing.\n");
		printf("Try adjusting its value.\n");
	}

	return 0;
}
