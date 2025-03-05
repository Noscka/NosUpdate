project ("NosUpdater" VERSION ${GIT_VERSION} LANGUAGES CXX)

file(GLOB_RECURSE UpdaterSource CONFIGURE_DEPENDS "Source/*")

add_executable(${PROJECT_NAME} "UpdaterMain.cpp" ${UpdaterSource})
target_link_libraries(${PROJECT_NAME} PRIVATE "NosUpdateLib")

#set(OUTDIRSUB ${OUTDIR}/Updater/)
#
#add_custom_command(
#   TARGET ${PROJECT_NAME} POST_BUILD 
#   COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTDIRSUB}
#   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${OUTDIRSUB}
#)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>"
	COMMAND_EXPAND_LISTS
)