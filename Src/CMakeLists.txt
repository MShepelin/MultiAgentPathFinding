set(HEADERS         config.h
                    environmentoptions.h
                    gl_const.h
                    nodes.h
                    search.h
                    searchresult.h
                    tinyxml2.h
                    map.h
                    agents.h
                    agents_array.h
                    nodes_heap.h)

set(SOURCES         config.cpp
                    environmentoptions.cpp
                    map.cpp
                    search.cpp
                    tinyxml2.cpp
                    nodes.cpp)

set(HEADERS_PP      ${HEADERS}
                    ilogger.h
                    xml_map.h
                    mission.h
                    xmllogger.h)

set(SOURCES_PP      ${SOURCES}
                    xml_map.cpp
                    mission.cpp
                    xmllogger.cpp)

set(HEADERS_MAPF    ${HEADERS}
                    tasker.h
                    scenarios.h
                    mapf/mapf_interface.h
					mapf/ma_star.h)

set(SOURCES_MAPF    ${SOURCES}
                    tasker.cpp
                    scenarios.cpp
					mapf/ma_star.cpp)

if (TESTS)

add_library(pathplanningbase ${HEADERS} ${SOURCES})

endif()


if (MAPF)

add_executable("${PROJECT_NAME}_MAPF" ${HEADERS_MAPF} ${SOURCES_MAPF} main_mapf.cpp)
add_library(MAPFLIB ${HEADERS_MAPF} ${SOURCES_MAPF})

install(TARGETS ${PROJECT_NAME}_MAPF
        CONFIGURATIONS Debug
        DESTINATION Bin/Debug)

install(TARGETS ${PROJECT_NAME}_MAPF
        CONFIGURATIONS Release
        DESTINATION Bin/Release)

target_include_directories("${PROJECT_NAME}_MAPF" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

else()

add_executable(${PROJECT_NAME} ${HEADERS_PP} ${SOURCES_PP} main.cpp)
add_library(PPLIB ${HEADERS_PP} ${SOURCES_PP})
install(TARGETS ${PROJECT_NAME} 
        CONFIGURATIONS Debug
        DESTINATION Bin/Debug)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        DESTINATION Bin/Release)

endif()
