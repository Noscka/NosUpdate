project ("NosUpdater" VERSION ${GIT_VERSION} LANGUAGES CXX)

add_executable(${PROJECT_NAME} "UpdaterMain.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE "NosUpdateLib")