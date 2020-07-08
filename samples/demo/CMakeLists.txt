cmake_minimum_required(VERSION 3.8.11)

set(TARGET demo)

project(${TARGET})

if(NOT TARGET docker_cpp)
	find_package( docker_cpp REQUIRED )
	find_package( ASL REQUIRED )
	find_package( LibArchive REQUIRED )
endif()

set(SRC
	demo.cpp
)

add_executable(${TARGET} ${SRC}) 

target_link_libraries ( ${TARGET} docker_cpp::docker asls LibArchive::LibArchive)

set_target_properties( ${TARGET} PROPERTIES FOLDER samples)
set_target_properties( ${TARGET} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})


add_executable(manage_image manage_images.cpp)
target_link_libraries (manage_image PRIVATE docker_cpp::docker asls LibArchive::LibArchive)
