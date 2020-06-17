#include <doctest/doctest.h>
#include "test_utils.h"

#include <docker_cpp/docker.h>

using namespace docker_cpp;

TEST_SUITE("IMAGE") {
    TEST_CASE("Check list_images returns a list of images") {
        Docker<MockResponseHttp> d("list_images");
        DockerError e = d.ping();
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }
}