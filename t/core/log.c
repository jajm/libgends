#include "log.h"

int main(int argc, char **argv)
{
	gds_log_init(GDS_LOG_LEVEL_DEBUG);

	GDS_LOG_FATAL("FATAL");
	GDS_LOG_ERROR("ERROR");
	GDS_LOG_WARNING("WARNING");
	GDS_LOG_NORMAL("NORMAL");
	GDS_LOG_INFO("INFO");
	GDS_LOG_DEBUG("DEBUG");

	GDS_LOG_NORMAL("%d+%d is %d", 3, 4, 3+4);

	return 0;
}
