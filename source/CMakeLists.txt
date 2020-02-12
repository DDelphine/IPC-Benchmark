# include the directory of header file
# equals to adding environment path to CPLUS_INCLUDE_PATH
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# -g: generate debug information when compiling
# -O3: optimize the code
# -Wall: output all warning information
# -Werror: transform the warning message to error message
add_compile_options(-g -O3 -Wall -Werror)

link_libraries(pthread)

add_subdirectory(common)
add_subdirectory(shm)
add_subdirectory(shm-hodor)
add_subdirectory(pipe)
add_subdirectory(domain)


