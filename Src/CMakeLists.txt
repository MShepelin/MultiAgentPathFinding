set(HEADERS_PP      config.h
                    environmentoptions.h
                    gl_const.h
                    ilogger.h
                    xml_map.h
                    mission.h
                    node.h
                    search.h
                    searchresult.h
                    tinyxml2.h
                    xmllogger.h)

set(SOURCES_PP      config.cpp
                    environmentoptions.cpp
                    xml_map.cpp
                    mission.cpp
                    search.cpp
                    tinyxml2.cpp
                    xmllogger.cpp)

set(HEADERS_MAPF    tasker.h
					mapf_interface.h
					agents.h
                    map.h
					scenaries.h)

set(SOURCES_MAPF    tasker.cpp
                    map.cpp
					scenaries.cpp)

add_executable(${PROJECT_NAME} ${HEADERS_PP} ${SOURCES_PP} main.cpp)

add_executable("${PROJECT_NAME}_MAPF" ${HEADERS_MAPF} ${SOURCES_MAPF} mapf_main.cpp)

add_library(PPLIB ${HEADERS_PP} ${SOURCES_PP})

add_library(MAPFLIB ${HEADERS_MAPF} ${SOURCES_MAPF})

install(TARGETS ${PROJECT_NAME} 
        CONFIGURATIONS Debug
        DESTINATION Bin/Debug)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        DESTINATION Bin/Release)
