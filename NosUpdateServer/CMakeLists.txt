project ("NosUpdateServer" VERSION ${GIT_VERSION} LANGUAGES CXX)

file(GLOB_RECURSE UpdateServerSource CONFIGURE_DEPENDS "Source/*")

add_executable(${PROJECT_NAME} "ServerMain.cpp" ${UpdateServerSource})
target_link_libraries(${PROJECT_NAME} PRIVATE "NosUpdateLib" Boost::thread)

set(OUTDIRSUB ${OUTDIR}/Server/)
file(GLOB_RECURSE CertPaths CONFIGURE_DEPENDS "${CERTDIR}/Server/*")

add_custom_command(
   TARGET ${PROJECT_NAME} POST_BUILD 
   COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTDIRSUB}
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${OUTDIRSUB}
   COMMAND ${CMAKE_COMMAND} -E copy -t ${OUTDIRSUB} ${CertPaths}
)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy -t ${OUTDIRSUB} "$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>"
	COMMAND_EXPAND_LISTS
)