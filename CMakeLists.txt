cmake_minimum_required(VERSION 3.2.2)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_TESTS "Build tests" ON)

include(cmake/HunterGate.cmake)

HunterGate(
    URL "https://github.com/uliana99/hunter/archive/v0.19.137.1.tar.gz"
    SHA1 "87af3ced13dd7a194ca94db4cd0d21908d80b596"
)


project(explorer)

hunter_add_package(Catch)
find_package(Catch CONFIG REQUIRED)

hunter_add_package(Boost COMPONENTS system filesystem)
find_package(Boost CONFIG REQUIRED system filesystem)

add_executable(explorer ${CMAKE_CURRENT_SOURCE_DIR}/sources/explorer.cpp)

target_include_directories(explorer PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(explorer Catch::Catch Boost::system Boost::filesystem)

install(TARGETS explorer RUNTIME DESTINATION bin)
