cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME "Dmitriy_Shepelin_ASearch")
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2 -Wall")

if (${MINGW})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -static -static-libgcc -static-libstdc++")
endif()

if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -static -static-libgcc -static-libstdc++")
endif()

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

option(MAPF "Build MAPF solver and use MAPF file formats" OFF)
option(TESTS "Enable testing" ON)

if (MSVC)
set(gtest_force_shared_crt ON)
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/googletest")
enable_testing()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Tests")