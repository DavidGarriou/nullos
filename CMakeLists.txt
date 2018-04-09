cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project("Nullos")

enable_language(ASM)

add_library(multiboot OBJECT "")
add_library(kernel OBJECT "")

include(src/CMakeLists.txt)

#set(SOURCE
#	${CMAKE_CURRENT_SOURCE_DIR}
#)

#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SUB_SOURCES)

#add_library(multiboot OBJECT multiboot.s)
target_compile_options(multiboot PRIVATE -m32 -Wall -Wextra)
#add_library(kernel OBJECT kernel.c)
target_compile_options(kernel PRIVATE -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra)

#set(LINKER_SCRIPT "linker.ld")
add_executable(nullos $<TARGET_OBJECTS:multiboot> $<TARGET_OBJECTS:kernel>)
set_target_properties(nullos PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")
target_link_libraries(nullos -m32 -O2 -ffreestanding -nostdlib -lgcc -Wl,--build-id=none)

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
	message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

# Get all propreties that cmake supports
execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)

# Convert command output into a CMake list
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")

function(print_properties)
	message ("CMAKE_PROPERTY_LIST = ${CMAKE_PROPERTY_LIST}")
endfunction(print_properties)

function(print_target_properties tgt)
	if(NOT TARGET ${tgt})
		message("There is no target named '${tgt}'")
		return()
	endif()
	foreach (prop ${CMAKE_PROPERTY_LIST})
		string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop ${prop})
		# Fix # https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i
		if(prop STREQUAL "LOCATION" OR prop MATCHES "^LOCATION_" OR prop MATCHES "_LOCATION$")
			continue()
		endif()
		# message # ("Checking # ${prop}")
		get_property(propval TARGET ${tgt} PROPERTY ${prop} SET)
		if (propval)
			get_target_property(propval ${tgt} ${prop})
			message ("${tgt} ${prop} = ${propval}")
		endif()
	endforeach(prop)
endfunction(print_target_properties)
