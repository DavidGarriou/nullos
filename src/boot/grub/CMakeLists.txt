target_sources(multiboot
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/multiboot.h"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/multiboot.s"
)
