cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

project("To Do List")

enable_language(ASM)

#SET_SOURCE_FILES_PROPERTIES(multiboot.S PROPERTIES LANGUAGE C)

enable_testing()

add_library(multiboot OBJECT multiboot.s)
target_compile_options(multiboot PRIVATE -m32 -Wall -Wextra)
add_library(kernel OBJECT kernel.c)
target_compile_options(kernel PRIVATE -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra)

set(LINKER_SCRIPT "linker3.ld")
add_executable(nullos $<TARGET_OBJECTS:multiboot> $<TARGET_OBJECTS:kernel>)
set_target_properties(nullos PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")
target_link_libraries(nullos -m32 -O2 -ffreestanding -nostdlib -lgcc -Wl,--build-id=none)

