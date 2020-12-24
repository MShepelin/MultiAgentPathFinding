cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME "Dmitriy_Shepelin_ASearch")
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2 -Wall")

if (UNIX AND NOT APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -static -static-libgcc -static-libstdc++")
endif()

if (${MINGW})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -static -static-libgcc -static-libstdc++")
endif()

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

enable_testing()

add_subdirectory(Src)
add_subdirectory(Tests)