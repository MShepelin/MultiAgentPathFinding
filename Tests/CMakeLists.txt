if (TESTS)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

get_target_property(BASE_SOURCE_DIR pathplanningbase SOURCE_DIR)
include_directories(${BASE_SOURCE_DIR})

add_executable(base_testing general_test.cpp)
target_link_libraries(base_testing 
		PRIVATE pathplanningbase
		PRIVATE gtest
		PRIVATE gtest_main)

if (MAPF)
	get_target_property(MAPFLIB_SOURCE_DIR MAPFLIB SOURCE_DIR) 
	include_directories(${MAPFLIB_SOURCE_DIR})
	
	add_executable(testbench mapf_test.cpp)

	target_link_libraries(testbench 
		PRIVATE MAPFLIB
		PRIVATE gtest
		PRIVATE gtest_main)

else()
	get_target_property(PPLIB_SOURCE_DIR PPLIB SOURCE_DIR) 
	include_directories(${PPLIB_SOURCE_DIR})

	add_executable(testbench pp_test.cpp)

	target_link_libraries(testbench PPLIB)

	add_test(NAME Test1 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/0_test.xml)
	add_test(NAME Test2 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/1_test.xml)
	add_test(NAME Test3 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/2_test.xml)
	add_test(NAME Test4 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/3_test.xml)
	add_test(NAME Test5 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/4_test.xml)
	add_test(NAME Test6 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/5_test.xml)
	add_test(NAME Test7 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/6_test.xml)
	add_test(NAME Test8 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/7_test.xml)
	add_test(NAME Test9 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/8_test.xml)
	add_test(NAME Test10 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/9_test.xml)
	add_test(NAME Test11 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/A_test.xml)
	add_test(NAME Test12 COMMAND testbench ${CMAKE_SOURCE_DIR}/TestData/B_test.xml)
endif()
endif()