#include <doctest/doctest.h>
#include "test_utils.h"

#include <docker_cpp/docker.h>

using namespace docker_cpp;

TEST_SUITE("SYSTEM") {
    TEST_CASE("Check PING returns OK") {
    Docker<MockInfoHttp> d("localhost");
    DockerError e = d.ping();
    CHECK(e.isOk() == true);
    CHECK(e.isError() == false);
    }

    TEST_CASE("Check PING handles ERROR") {
        Docker<MockErrorHttp> d("localhost");
        DockerError e = d.ping();
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check VERSION parses result") {
        CHECK(0 == 2);
    }

    TEST_CASE("Check VERSION handles ERROR") {
        CHECK(0 == 1);
    }
}