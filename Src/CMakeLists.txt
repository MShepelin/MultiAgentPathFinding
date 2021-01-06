add_compile_definitions(CONSOLE_BUILD)

set(HEADERS         config.h
                    environmentoptions.h
                    gl_const.h
                    ilogger.h
                    map.h
                    mission.h
                    node.h
                    search.h
                    searchresult.h
                    tinyxml2.h
                    xmllogger.h)

set(SOURCES         config.cpp
                    environmentoptions.cpp
                    map.cpp
                    mission.cpp
                    search.cpp
                    tinyxml2.cpp
                    xmllogger.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} main.cpp)

add_library(PPLIB ${HEADERS} ${SOURCES})

install(TARGETS ${PROJECT_NAME} 
        CONFIGURATIONS Debug
        DESTINATION Bin/Debug)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        DESTINATION Bin/Release)
