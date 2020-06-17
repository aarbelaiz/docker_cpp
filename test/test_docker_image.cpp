#include <doctest/doctest.h>

#include "test_utils.h"

using namespace docker_cpp;

TEST_SUITE("IMAGE") {
    TEST_CASE("Check list_images returns a list of images") {
        Docker<MockResponseHttp> d("images");
        ImageList r;
        DockerError e = d.images(r);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(r.size() > 0);
    }
}