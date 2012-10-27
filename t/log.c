#include "log.h"

int main(void)
{
	gds_log_init(GDS_LOG_LEVEL_DEBUG);

	GDS_LOG_FATAL("fatal message");
	GDS_LOG_ERROR("error message");
	GDS_LOG_WARNING("warning message");
	GDS_LOG_INFO("info message");
	GDS_LOG_DEBUG("debug message");

	GDS_LOG_INFO("%d+%d = %d", 3, 4, 3+4);

	return 0;
}
