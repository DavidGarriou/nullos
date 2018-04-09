include(${CMAKE_CURRENT_LIST_DIR}/boot/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/kernel/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/drivers/CMakeLists.txt)

set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/linker.ld")

