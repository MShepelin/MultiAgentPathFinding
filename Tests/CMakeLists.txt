get_target_property(MAPFLIB_SOURCE_DIR MAPFLIB SOURCE_DIR) 
include_directories(${MAPFLIB_SOURCE_DIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(testbench testbench.cpp)

target_link_libraries(
	testbench 
	PRIVATE MAPFLIB
	PRIVATE gtest 
	PRIVATE gtest_main
)

if (MSVC)
	set(gtest_force_shared_crt ON)
endif()
