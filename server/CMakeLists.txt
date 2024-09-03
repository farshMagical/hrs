cmake_minimum_required(VERSION 3.22)

set(TARGET hrs_server) 

set(CMAKE_BUILD_TYPE "Debug")

set(SRC_DIRS
    ${CMAKE_SOURCE_DIR}/server
    ${CMAKE_SOURCE_DIR}/server/hrs
    ${CMAKE_SOURCE_DIR}/server/hrs/calculation
    ${CMAKE_SOURCE_DIR}/server/utils
)

include_directories(
    ${SRC_DIRS}
)

foreach(DIR ${SRC_DIRS})
    aux_source_directory(${DIR} SRCS)
endforeach()

add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} PRIVATE fmt::fmt Boost::beast nlohmann_json::nlohmann_json)
