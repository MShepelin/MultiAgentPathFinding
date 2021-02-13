set(HEADERS_PP      config.h
                    mapf_interface.h
                    environmentoptions.h
                    gl_const.h
                    ilogger.h
                    xml_map.h
                    mission.h
                    node.h
                    search.h
                    searchresult.h
                    tinyxml2.h
                    map.h
                    xmllogger.h
                    agents.h)

set(SOURCES_PP      config.cpp
                    environmentoptions.cpp
                    map.cpp
                    xml_map.cpp
                    mission.cpp
                    search.cpp
                    tinyxml2.cpp
                    xmllogger.cpp)

set(HEADERS_MAPF    tinyxml2.h
                    tasker.h
                    search.h
                    environmentoptions.h
                    gl_const.h
                    config.h
                    searchresult.h
                    node.h
                    mapf_interface.h
                    agents.h
                    map.h
                    scenaries.h)

set(SOURCES_MAPF    search.cpp
                    tinyxml2.cpp
                    tasker.cpp
                    map.cpp
                    environmentoptions.cpp
                    scenaries.cpp)

if (MAPF)

add_compile_definitions(MAPF)
add_executable("${PROJECT_NAME}_MAPF" ${HEADERS_MAPF} ${SOURCES_MAPF} mapf_main.cpp)
add_library(MAPFLIB ${HEADERS_MAPF} ${SOURCES_MAPF})

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
