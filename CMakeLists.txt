cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(blackjack CXX)

###############################################################################

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################

set(COMMON
    main.cpp
    shell.cpp
    blackjack.cpp
    probabilities.cpp
)
list(TRANSFORM COMMON PREPEND "src/")

###############################################################################

add_executable(blackjack ${COMMON})
target_include_directories(blackjack PRIVATE ${PROJECT_SOURCE_DIR}/include/)
target_link_libraries(blackjack PRIVATE )
target_compile_options(blackjack PRIVATE -Wall -Wextra -Werror)
set_target_properties(blackjack PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

###############################################################################
