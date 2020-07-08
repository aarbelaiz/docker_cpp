#include <doctest/doctest.h>
#include "test_utils.h"

using namespace docker_cpp;

TEST_SUITE("CONTAINER") {
    TEST_CASE("Check containerList returns a list of containers") {
        Docker<MockResponseHttp> d("container_list");
        ContainerList r;
        DockerError e = d.containerList(r);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(r.size() > 0);
        CHECK(r[0].id == "8dfafdbc3a40");
        CHECK(r[0].names.empty() == false);
        CHECK(r[0].image == "ubuntu:latest");
        CHECK(r[0].imageID == "d74508fb6632491cea586a1fd7d748dfc5274cd6fdfedee309ecdcbc2bf5cb82");
        CHECK(r[0].command == "echo 1");
        CHECK(r[0].created == 1367854155);
        CHECK(r[0].state == "Exited");
        CHECK(r[0].status == "Exit 0");
        CHECK(r[0].ports.empty() == false);
        CHECK(r[0].ports[0].privatePort == 2222);
        CHECK(r[0].ports[0].publicPort == 3333);
        CHECK(r[0].ports[0].type == "tcp");
        CHECK(r[0].labels.empty() == false);
        CHECK(r[0].labels.size() == 1);
        CHECK(r[0].labels[0].first == "com.example.vendor");
        CHECK(r[0].labels[0].second == "Acme");
        CHECK(r[0].sizeRw == 12288);
        CHECK(r[0].sizeRootFs == 0);
        CHECK(r[0].hostConfig.first == "NetworkMode");
        CHECK(r[0].hostConfig.second == "default");
        CHECK(r[0].networkSettings.networks[0].first == "bridge");
        CHECK(r[0].networkSettings.networks[0].second.gateway == "172.17.0.1");
        CHECK(r[0].networkSettings.networks[0].second.networkID == "7ea29fc1412292a2d7bba362f9253545fecdfa8ce9a6e37dd10ba8bee7129812");
        CHECK(r[0].networkSettings.networks[0].second.endpointID == "2cdc4edb1ded3631c81f57966563e5c8525b81121bb3706a9a9a3ae102711f3f");
        CHECK(r[0].networkSettings.networks[0].second.ipAddress == "172.17.0.2");
        CHECK(r[0].networkSettings.networks[0].second.ipPrefixLen == 16);
        CHECK(r[0].networkSettings.networks[0].second.ipV6Gateway == "");
        CHECK(r[0].networkSettings.networks[0].second.globalIpV6Address == "");
        CHECK(r[0].networkSettings.networks[0].second.globalIpV6PrefixLen == 0);
        CHECK(r[0].networkSettings.networks[0].second.macAddress == "02:42:ac:11:00:02");
        CHECK("r[0].mounts.empty() == false" == "TODO");
    }

    TEST_CASE("Check containerList handles error") {
        Docker<MockErrorHttp> d("500");
        ContainerList r;
        DockerError e = d.containerList(r);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
        CHECK(r.empty() == true);
    }

    TEST_CASE("Check containerStart returns OK") {
        Docker<MockResponseHttp> d("200");
        DockerError e = d.containerStart("containerID");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check containerStart when container is already started") {
        Docker<MockErrorHttp> d("304");
        DockerError e = d.containerStart("containerID");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check containerStart handles error") {
        Docker<MockErrorHttp> d("500");
        DockerError e = d.containerStart("containerID");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerStop returns OK") {
        Docker<MockErrorHttp> d("204");
        DockerError e = d.containerStop("containerID");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check containerStop when container is already stopped") {
        Docker<MockErrorHttp> d("304");
        DockerError e = d.containerStop("containerID");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check containerStop handles error") {
        Docker<MockErrorHttp> d("500");
        DockerError e = d.containerStop("containerID");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerRestart returns OK") {
        Docker<MockErrorHttp> d("204");
        DockerError e = d.containerRestart("containerID");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check containerRestart handles error") {
        Docker<MockErrorHttp> d("500");
        DockerError e = d.containerRestart("containerID");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerKill returns no error") {
        Docker<MockErrorHttp> d("204");
        DockerError e = d.containerKill("containerID");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check containerKill return error when it is not running") {
        Docker<MockErrorHttp> d("409");
        DockerError e = d.containerKill("containerID");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerKill handles error") {
        Docker<MockErrorHttp> d("500");
        DockerError e = d.containerKill("containerID");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerRename return OK") {
        Docker<MockErrorHttp> d("204");
        DockerError e = d.containerRename("containerID", "foo");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check containerRename returns error when there is no such container") {
        Docker<MockErrorHttp> d("404");
        DockerError e = d.containerRename("containerID", "foo");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerRename returns error when name is already in use") {
        Docker<MockErrorHttp> d("409");
        DockerError e = d.containerRename("containerID", "foo");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerRename handles error") {
        Docker<MockErrorHttp> d("500");
        DockerError e = d.containerRename("containerID", "foo");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerWait parses OK response") {
        Docker<MockResponseHttp> d("container_wait");
        WaitInfo r;
        DockerError e = d.containerWait("containerID", r);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(r.statusCode == 0);
        CHECK(r.errorMsg == "string");
    }

    TEST_CASE("Check containerWait returns error when container does not exist") {
        Docker<MockErrorHttp> d("404");
        WaitInfo r;
        DockerError e = d.containerWait("containerID", r);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }


    TEST_CASE("Check containerWait handles error") {
        Docker<MockErrorHttp> d("500");
        WaitInfo r;
        DockerError e = d.containerWait("containerID", r);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerRemove returns OK") {
        Docker<MockErrorHttp> d("204");
        WaitInfo r;
        DockerError e = d.containerRemove("containerID");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check containerRemove handles bad input parameter") {
        Docker<MockErrorHttp> d("400");
        WaitInfo r;
        DockerError e = d.containerRemove("containerID");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check containerRemove handles error") {
        Docker<MockErrorHttp> d("500");
        WaitInfo r;
        DockerError e = d.containerRemove("containerID");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }
}