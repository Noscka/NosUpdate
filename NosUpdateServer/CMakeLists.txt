project ("NosUpdateServer" VERSION ${GIT_VERSION} LANGUAGES CXX)

add_executable(${PROJECT_NAME} "ServerMain.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE "NosUpdateLib" Boost::thread)