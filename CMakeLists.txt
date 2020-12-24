cmake_minimum_required(VERSION 3.2)
set(PROJECT_NAME "Dmitriy_Shepelin_ASearch")
project(${PROJECT_NAME})

# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
	if (${MINGW})
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2 -Wall -Wextra -static -static-libgcc -static-libstdc++")
	else ()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2 -Wall -Wextra")
	endif()
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2 -Wall")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

enable_testing()

add_subdirectory(Src)
add_subdirectory(Tests)