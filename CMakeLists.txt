cmake_minimum_required (VERSION 2.6)

project (nullos LANGUAGES C)
# The version number.
set (nullos_VERSION_MAJOR 1)
set (nullos_VERSION_MINOR 0)
set (PROJECT_SOURCE_DIR ..)

SET(TARGET_nullos_NAME nullos)
SET(TARGET_nullos_FILES
${PROJECT_SOURCE_DIR}/src/boot/grub/multiboot.S
${PROJECT_SOURCE_DIR}/src/boot/grub/multiboot.h
${PROJECT_SOURCE_DIR}/src/kernel/kernel.c)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#	"${PROJECT_SOURCE_DIR}/nullosConfig.h.in"
#	"${PROJECT_BINARY_DIR}/nullosConfig.h"
#)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#include_directories("${PROJECT_BINARY_DIR}")

add_executable({TARGET_nullos_NAME}
${TARGET_nullos_FILES})
#TARGET_LINK_LIBRARIES(
#	${TARGET_nullos_NAME}
#	${TARGET_nullos_LIBS})

#set(EXECUTABLE_OUTPUT_PATH ./build)

