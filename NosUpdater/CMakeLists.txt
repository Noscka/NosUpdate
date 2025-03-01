project ("NosUpdater" VERSION ${GIT_VERSION} LANGUAGES CXX)

add_executable(${PROJECT_NAME} "UpdaterMain.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE "NosUpdateLib")

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>"
	COMMAND_EXPAND_LISTS
)