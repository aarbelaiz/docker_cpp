set(TARGET docker)

project(${TARGET})

# External dependencies
find_package(ASL REQUIRED)

set(SRC
	docker_error.cpp
	docker_parse.cpp
	docker_utils.cpp
)

set(INC ../include/docker_cpp)

set(HEADERS
	${INC}/docker.h
	${INC}/docker_types.h
	${INC}/docker_parse.h
	${INC}/docker_http.h
	${INC}/docker_error.h
	${INC}/docker_utils.h
	${INC}/export.h
)

add_library(${TARGET} SHARED ${SRC} ${HEADERS}) 

set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER "${HEADERS}")
target_include_directories(${TARGET} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
											$<INSTALL_INTERFACE:$<TARGET_FILE_DIR:${TARGET}>/../include>)

target_link_libraries(${TARGET} PUBLIC asls)

#export(TARGETS ${TARGET} FILE ${CMAKE_BINARY_DIR}/${TARGET}Config.cmake )

install(TARGETS ${TARGET}
	EXPORT ${LIB_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include/${INC}
)
