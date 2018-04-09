target_sources(kernel
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/kernel.h"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/kernel.c"
)

