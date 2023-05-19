cmake_minimum_required(VERSION 3.22.2)
project(tree_interp)

# Export compile commands
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use modern C++
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

include_directories("${PROJECT_BINARY_DIR}" libs src)

# Making executable
# Original sources glob 
file(GLOB_RECURSE source
    ${CMAKE_CURRENT_LIST_DIR}/src/*\.[ch]
    ${CMAKE_CURRENT_LIST_DIR}/src/*\.[ch]pp
    )
file(GLOB libs libs/*.h libs/*.hpp libs/*.cpp libs/*.c libs/imgui/*.h libs/imgui/*.cpp)
file(GLOB sources ${source} ${libs})

add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${DEFINITIONS})
# FOR DEBUGGING
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic) 
# FOR PERFORMANCE
target_compile_options(${PROJECT_NAME} PRIVATE -O3) 

