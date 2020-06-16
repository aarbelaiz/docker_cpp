set(TARGET test)

project(${TARGET})

include(FetchContent)

# DOCTEST
FetchContent_Declare(
    doctest
    URL "https://github.com/onqtam/doctest/archive/2.3.8.tar.gz"
    URL_MD5 6a4a50777596c9db6515b7e8b38fbce2)
FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
    FetchContent_Populate(doctest)
    #add_definitions(-DDOCTEST_CONFIG_DISABLE)
endif()

set(SRC test_docker.cpp test_docker_system.cpp)
set(HEADERS test_utils.h)

add_executable(${TARGET} ${SRC} ${HEADERS})
target_include_directories(${TARGET} PUBLIC ${doctest_SOURCE_DIR})
target_link_libraries(${TARGET} docker)
