project(MULTICAST_SERVER)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)
set(PACKETIZER_SOURCE_DIR ../packetizer)
set(UTILS_SOURCE_DIR ../utils)


if(UNIX)
include_directories(
	${CURL_INCLUDE_DIR} 
    ${LIBTORRENT_INCLUDE_DIR}
)
ENDIF(UNIX)

include_directories(
    ${PACKETIZER_SOURCE_DIR}/include
    ${UTILS_SOURCE_DIR}/include
)

find_package(Boost COMPONENTS system thread log REQUIRED)

set(SOURCE
    ${MULTICAST_SERVER_SOURCE_DIR}/src/server.cpp
    ${MULTICAST_SERVER_SOURCE_DIR}/src/file_reader.cpp
    ${MULTICAST_SERVER_SOURCE_DIR}/src/multicast_server.cpp
    ${PACKETIZER_SOURCE_DIR}/src/packetizer.cpp
    ${PACKETIZER_SOURCE_DIR}/src/movie_packet.cpp
    ${PACKETIZER_SOURCE_DIR}/src/sync_packet.cpp
)

set(HEADERS
    ${MULTICAST_SERVER_SOURCE_DIR}/include/server.hpp
    ${MULTICAST_SERVER_SOURCE_DIR}/include/file_reader.hpp
	 ${PACKETIZER_SOURCE_DIR}/include/packetizer.hpp
	 ${PACKETIZER_SOURCE_DIR}/include/movie_packet.hpp
	 ${PACKETIZER_SOURCE_DIR}/include/sync_packet.hpp
)

include_directories(
    ${MULTICAST_SERVER_SOURCE_DIR}/include
    ${MULTICAST_SERVER_SOURCE_DIR}/../libcow/include/
    ${Boost_INCLUDE_DIRS}
)

link_directories(
    ${Boost_LIBRARY_DIRS}
)
                 
add_executable(multicast_server ${SOURCE} ${HEADERS})

IF(UNIX)    
target_link_libraries(multicast_server 
    ${CURL_LIBRARY} 
    ${LIBTORRENT_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
	 ${Boost_LOG_LIBRARY}
)
ENDIF(UNIX)