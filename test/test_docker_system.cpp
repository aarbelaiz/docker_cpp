#include <doctest/doctest.h>
#include "test_utils.h"

using namespace docker_cpp;

TEST_SUITE("SYSTEM") {
    TEST_CASE("Check PING returns OK") {
        Docker<MockResponseHttp> d("ping");
        DockerError e = d.ping();
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check PING handles ERROR") {
        Docker<MockErrorHttp> d("mocked");
        DockerError e = d.ping();
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check connection method success") {
        Docker<MockResponseHttp> d("200");
        bool r = d.checkConnection();
        CHECK(r == true);
    }

    TEST_CASE("Check connection method fails") {
        Docker<MockErrorHttp> d("mocked");
        bool r = d.checkConnection();
        CHECK(r == false);
    }

    TEST_CASE("Check VERSION parses result") {
        Docker<MockResponseHttp> d("version");
        VersionInfo i;
        DockerError e = d.version(i);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(i.version == "17.04.0");
        CHECK(i.os == "linux");
        CHECK(i.kernelVersion == "3.19.0-23-generic");
        CHECK(i.goVersion == "go1.7.5");
        CHECK(i.gitCommit == "deadbee");
        CHECK(i.arch == "amd64");
        CHECK(i.apiVersion == "1.27");
        CHECK(i.minApiVersion == "1.12");
        CHECK(i.buildTime == "2016-06-14T07:09:13.444803460+00:00");
        CHECK(i.experimental == true);
    }

    TEST_CASE("Check VERSION handles ERROR") {
        Docker<MockErrorHttp> d("mock");
        VersionInfo i;
        DockerError e = d.version(i);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }
}