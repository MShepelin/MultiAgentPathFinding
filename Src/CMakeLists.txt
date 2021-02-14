set(HEADERS         config.h
                    mapf_interface.h
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
                    scenaries.h)

set(SOURCES_MAPF    ${SOURCES}
                    tasker.cpp
                    scenaries.cpp)

if (TESTS)

add_library(pathplanningbase ${HEADERS} ${SOURCES})

endif()


if (MAPF)

add_executable("${PROJECT_NAME}_MAPF" ${HEADERS_MAPF} ${SOURCES_MAPF} mapf_main.cpp)
add_library(MAPFLIB ${HEADERS_MAPF} ${SOURCES_MAPF})

install(TARGETS ${PROJECT_NAME}_MAPF
        CONFIGURATIONS Debug
        DESTINATION Bin/Debug)

install(TARGETS ${PROJECT_NAME}_MAPF
        CONFIGURATIONS Release
        DESTINATION Bin/Release)

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
