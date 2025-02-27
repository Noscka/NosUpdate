project ("TesterLib" VERSION 0.0.1 LANGUAGES CXX)

# Tester Lib
file(GLOB_RECURSE TesterLibSource CONFIGURE_DEPENDS "Source/*")

add_library(${PROJECT_NAME} ${TesterLibSource})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(${PROJECT_NAME} PUBLIC "NosUpdateLib" NosLib::NosLib Boost::serialization)

if(WIN32 AND BUILD_SHARED_LIBS)
  set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()