project(COWCAST)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)
set(COW_UTILS_SOURCE_DIR ${COWCAST_SOURCE_DIR}/libs/cow_utils)
include(ExternalProject)

file(COPY ${COWCAST_SOURCE_DIR}/src/start_multicast_server.sh DESTINATION .)
find_package(Boost COMPONENTS system thread log REQUIRED)

ExternalProject_Add(
    cow_utils
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${COW_UTILS_SOURCE_DIR}
    BINARY_DIR ${COW_UTILS_SOURCE_DIR}/build
    INSTALL_COMMAND ""
)

set(SOURCE
    ${COWCAST_SOURCE_DIR}/src/server.cpp
    ${COWCAST_SOURCE_DIR}/src/file_reader.cpp
    ${COWCAST_SOURCE_DIR}/src/multicast_server.cpp
)

set(HEADERS
    ${COWCAST_SOURCE_DIR}/include/server.hpp
    ${COWCAST_SOURCE_DIR}/include/file_reader.hpp
)

include_directories(
    ${COWCAST_SOURCE_DIR}/include
    ${COW_UTILS_SOURCE_DIR}/buffer/include
    ${COW_UTILS_SOURCE_DIR}/packetizer/include
    ${Boost_INCLUDE_DIRS}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
)
                 
add_executable(cowcast ${SOURCE} ${HEADERS})

IF(UNIX)    
    target_link_libraries(cowcast 
        ${Boost_REGEX_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${COW_UTILS_SOURCE_DIR}/build/libcow_utils.so
    )
ENDIF(UNIX)
