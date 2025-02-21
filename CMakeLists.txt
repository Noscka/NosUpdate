cmake_minimum_required (VERSION 3.12)

include("CMake/Versioning.cmake")

Get_Git_Version()
Get_Git_Branch()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("NosUpdateLib" VERSION ${GIT_VERSION} LANGUAGES CXX)

# Version Header
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/CMake/Version.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/Headers/Version.hpp")

find_package(NosLib REQUIRED)

add_subdirectory(NosUpdateLib)

add_subdirectory(NosUpdateServer)

if(PROJECT_IS_TOP_LEVEL)
	add_subdirectory("NosUpdateTester")
endif()