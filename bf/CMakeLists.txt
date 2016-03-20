cmake_minimum_required(VERSION 2.8.11)

if(NOT CMAKE_PROJECT_NAME)
    project(bf)
endif()

set(BF_SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

include_directories(
    .
    ${BF_SRC_DIR}/../asmjit/src
)

link_directories(
    ${BF_SRC_DIR}/../asmjit/build_makefiles_rel
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" -L\"../asmjit/build_makefiles_rel\" ")

# Define the CXX sources
set ( CXX_SRCS
    ${BF_SRC_DIR}/main.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -g -O0 -Wall")

add_executable(bf ${CXX_SRCS} ${C_SRCS})

target_link_libraries(bf
    asmjit
)

add_custom_command(TARGET bf
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory res
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${BF_SRC_DIR}/res res
	)

add_custom_target(hello ./bf res/hello.bf)
add_custom_target(simple ./bf res/simple.bf < res/simple_in)
add_custom_target(rot13 ./bf res/rot13.bf < res/rot13_in)
