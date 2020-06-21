#include <doctest/doctest.h>
#include "test_utils.h"

using namespace docker_cpp;

TEST_SUITE("IMAGE") {
    TEST_CASE("Check image_list returns a list of images") {
        Docker<MockResponseHttp> d("image_list");
        ImageList r;
        DockerError e = d.imageList(r);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(r.size() > 0);
        CHECK(r[0].id == "sha256:e216a057b1cb1efc11f8a268f37ef62083e70b1b38323ba252e25ac88904a7e8");
        CHECK(r[0].parentId == "");
        CHECK(r[0].repoTags.size() > 0);
        CHECK(r[0].repoTags[0] == "ubuntu:12.04");
        CHECK(r[0].repoTags[1] == "ubuntu:precise");
        CHECK(r[0].repoDigests.size() > 0);
        CHECK(r[0].repoDigests[0] == "ubuntu@sha256:992069aee4016783df6345315302fa59681aae51a8eeb2f889dea59290f21787");
        CHECK(r[0].created == 1474925151);
        CHECK(r[0].size == 103579269);
        CHECK(r[0].virtualSize == 103579269);
        CHECK(r[0].sharedSize == 0);
        CHECK(r[0].labels.empty() == true);
        CHECK(r[0].containers == 2);
    }

    TEST_CASE("Check image_list handles error response") {
        Docker<MockErrorHttp> d("500");
        ImageList r;
        DockerError e = d.imageList(r);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
        CHECK(e.msg.empty() == false);
    }

    TEST_CASE("Check image_tag returns OK") {
        Docker<MockResponseHttp> d("image_tag");
        DockerError e = d.imageTag("foo", "bar", "test");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check image_tag handles Errors") {
        Docker<MockErrorHttp> d("500");
        ImageList r;
        DockerError e = d.imageTag("foo", "bar", "test");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
        CHECK(e.msg.empty() == false);
    }

    TEST_CASE("Check image_remove returns OK") {
        Docker<MockResponseHttp> d("image_remove");
        DeletedImageList r;
        DockerError e = d.imageRemove("foo", r);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(r.size() > 0);
        CHECK(r[0].untagged == "3e2f21a89f");
        CHECK(r[1].deleted == "3e2f21a89f");
    }

    TEST_CASE("Check image_remove handles Errors") {
        Docker<MockErrorHttp> d("500");
        DeletedImageList r;
        DockerError e = d.imageRemove("foo", r);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
        CHECK(e.msg.empty() == false);
    }

    TEST_CASE("Check image_prune returns OK") {
        Docker<MockResponseHttp> d("image_prune");
        PruneInfo r;
        DockerError e = d.imagePrune("foo", r);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(r.imagesDeleted.size() > 0);
        CHECK(r.imagesDeleted[0].untagged == "string");
        CHECK(r.imagesDeleted[0].deleted == "string");
        CHECK(r.spaceReclaimed == 0);
    }

    TEST_CASE("Check image_prune handles Errors") {
        Docker<MockErrorHttp> d("500");
        PruneInfo r;
        DockerError e = d.imagePrune("foo", r);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
        CHECK(e.msg.empty() == false);
    }
}