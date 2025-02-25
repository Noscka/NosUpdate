cmake_minimum_required (VERSION 3.12)

include("CMake/Versioning.cmake")

Get_Git_Version()
Get_Git_Branch()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

add_compile_definitions(WIN32_LEAN_AND_MEAN)
project ("NosUpdate" VERSION ${GIT_VERSION} LANGUAGES CXX)

# Version Header
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/CMake/Version.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/Headers/Version.hpp")

if(UNIX)
	set(NosLib_DIR "$ENV{HOME}/.local/lib/cmake/NosLib")
	set(httplib_DIR "$ENV{HOME}/.local/lib/cmake/httplib")
endif()

find_package(NosLib REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread serialization)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_subdirectory(NosUpdateLib)

add_subdirectory(NosUpdateServer)

add_subdirectory(NosUpdater)

if(PROJECT_IS_TOP_LEVEL)
	add_subdirectory("NosUpdateTester")
endif()