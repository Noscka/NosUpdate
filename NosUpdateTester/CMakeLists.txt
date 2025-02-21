project ("NosUpdateTester" VERSION 0.0.1 LANGUAGES CXX)

add_executable(${PROJECT_NAME} "TesterMain.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE "NosUpdateLib")