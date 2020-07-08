#include <doctest/doctest.h>
#include "test_utils.h"

using namespace docker_cpp;

TEST_SUITE("VOLUMES") {
    TEST_CASE("Check volumeList returns OK and parses list of volumes") {
        Docker<MockResponseHttp> d("volumes_list");
        VolumeList l;
        DockerError e = d.volumesList(l);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(l.volumes.empty() == false);
        CHECK(l.volumes[0].name == "tardis");
        CHECK(l.volumes[0].driver == "local");
        CHECK(l.volumes[0].createdAt == "2017-07-19T12:00:26Z");
        CHECK(l.volumes[0].mountpoint == "/var/lib/docker/volumes/tardis");
        CHECK(l.volumes[0].scope == "local");
        CHECK(l.volumes[0].labels.size() == 2);
        CHECK(l.volumes[0].labels["com.example.some-label"] == "some-value");
        CHECK(l.volumes[0].driverOptions.size() == 3);
        CHECK(l.volumes[0].driverOptions["device"] == "tmpfs");
        CHECK(l.volumes[0].driverOptions["o"] == "size=100m,uid=1000");
        CHECK(l.warnings.empty() == true);
    }

    TEST_CASE("Check volumeList handles ERROR") {
        Docker<MockErrorHttp> d("500");
        VolumeList l;
        DockerError e = d.volumesList(l);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check volumeInspect returns OK and parses volume data") {
        Docker<MockResponseHttp> d("volumes_inspect");
        VolumeInfo i;
        DockerError e = d.volumesInspect("foo", i);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(i.name == "tardis");
        CHECK(i.driver == "custom");
        CHECK(i.createdAt == "2016-06-07T20:31:11.853781916Z");
        CHECK(i.mountpoint == "/var/lib/docker/volumes/tardis");
        CHECK(i.scope == "local");
        CHECK(i.labels.size() == 2);
        CHECK(i.labels["com.example.some-label"] == "some-value");
        CHECK(i.status.size() == 1);
        CHECK(i.status["hello"] == "world");
    }


    TEST_CASE("Check volumeInspect handles ERROR") {
        Docker<MockErrorHttp> d("500");
        VolumeInfo i;
        DockerError e = d.volumesInspect("foo", i);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check volumeCreate returns OK and parses volume data") {
        Docker<MockResponseHttp> d("volumes_create");
        VolumeInfo i;
        VolumeBase v;
        DockerError e = d.volumesCreate(v, i);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(i.name == "tardis");
        CHECK(i.driver == "custom");
        CHECK(i.createdAt == "2016-06-07T20:31:11.853781916Z");
        CHECK(i.mountpoint == "/var/lib/docker/volumes/tardis");
        CHECK(i.scope == "local");
        CHECK(i.labels.size() == 2);
        CHECK(i.labels["com.example.some-label"] == "some-value");
        CHECK(i.status.size() == 1);
        CHECK(i.status["hello"] == "world");
    }


    TEST_CASE("Check volumeInspect handles ERROR") {
        Docker<MockErrorHttp> d("500");
        VolumeInfo i;
        VolumeBase v;
        DockerError e = d.volumesCreate(v, i);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check volumeRemove handles OK") {
        Docker<MockErrorHttp> d("204");
        DockerError e = d.volumesRemove("foo");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check volumeRemove handles ERROR") {
        Docker<MockErrorHttp> d("500");
        DockerError e = d.volumesRemove("foo");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check volumesDeleteUnused returns OK and parses result") {
        Docker<MockResponseHttp> d("volumes_prune");
        DeletedVolumesInfo i;
        DockerError e = d.volumesDeleteUnused(i);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(i.spaceReclaimed == 100);
        CHECK(i.volumesDeleted.size() == 1);
        CHECK(i.volumesDeleted[0] == "string");
    }

    TEST_CASE("Check volumeInspect handles ERROR") {
        Docker<MockErrorHttp> d("500");
        DeletedVolumesInfo i;
        DockerError e = d.volumesDeleteUnused(i);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }
}