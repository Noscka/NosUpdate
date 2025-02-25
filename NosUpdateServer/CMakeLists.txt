project ("NosUpdateServer" VERSION ${GIT_VERSION} LANGUAGES CXX)

file(GLOB_RECURSE UpdateServerSource CONFIGURE_DEPENDS "Source/*")

add_executable(${PROJECT_NAME} "ServerMain.cpp" ${UpdateServerSource})
target_link_libraries(${PROJECT_NAME} PRIVATE "NosUpdateLib" Boost::thread)