cmake_minimum_required (VERSION 3.12)

include("CMake/Versioning.cmake")
include("CMake/CPM.cmake")

Get_Git_Version()
Get_Git_Branch()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
	set(Boost_USE_STATIC_LIBS ON) 
else()
	set(Boost_USE_STATIC_LIBS OFF) 
endif()

set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
set(BUILD_SHARED_LIBS ON)

add_compile_definitions(WIN32_LEAN_AND_MEAN)

if(UNIX)
	set(NosLib_DIR "$ENV{HOME}/.local/lib/cmake/NosLib")
	set(httplib_DIR "$ENV{HOME}/.local/lib/cmake/httplib")
endif()

find_package(NosLib REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread serialization filesystem)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
find_package(OpenSSL REQUIRED Crypto SSL)
#CPMAddPackage("gh:rikyoz/bit7z@4.0.9") #Not even used right now

add_subdirectory(NosUpdateLib)

set(OUTDIR ${PROJECT_SOURCE_DIR}/Out/)
set(CERTDIR ${PROJECT_SOURCE_DIR}/Extra/)

add_subdirectory(NosUpdateServer)

add_subdirectory(NosUpdater)

if(PROJECT_IS_TOP_LEVEL)
	add_subdirectory(NosUpdateTester)
endif()