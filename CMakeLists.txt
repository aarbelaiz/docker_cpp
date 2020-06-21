cmake_minimum_required(VERSION 2.8.11)

set(LIB_NAME docker_cpp)

project(${LIB_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_DEBUG_POSTFIX "d")

add_subdirectory(src)

option(DOCKERCPP_BUILD_SAMPLES "Build samples" ON)
option(DOCKERCPP_BUILD_TESTS   "Build tests" OFF)

if(DOCKERCPP_BUILD_SAMPLES)
	add_subdirectory(samples)
endif()

if(DOCKERCPP_BUILD_TESTS)
	add_subdirectory(test)
endif()

if(CMAKE_SOURCE_DIR STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	export( PACKAGE ${LIB_NAME} )
endif()

export(TARGETS docker NAMESPACE ${LIB_NAME}:: FILE ${LIB_NAME}Config.cmake )

install(EXPORT ${LIB_NAME} DESTINATION cmake FILE ${LIB_NAME}Config.cmake )

