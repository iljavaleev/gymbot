
cmake_minimum_required(VERSION 3.17)
project(gymbot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)


# /usr/local/include/pqxx
set(PQXX /opt/homebrew/include/pqxx)
find_library(PQXX_LIB pqxx)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

if(NOT TARGET spdlog)
    find_package(spdlog REQUIRED)
endif()

find_package(CURL)
include_directories(/usr/local/include /opt/homebrew/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

add_subdirectory(bot)
add_executable(gymbot gymbot.cc)

target_link_libraries(gymbot bot /usr/local/lib/libTgBot.a ${PQXX_LIB} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES}
${Boost_LIBRARIES} spdlog::spdlog)