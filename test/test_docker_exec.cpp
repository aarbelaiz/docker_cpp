#include <doctest/doctest.h>
#include "test_utils.h"

using namespace docker_cpp;

TEST_SUITE("EXEC") {
    TEST_CASE("Check creation of an exec instances returns OK") {
        Docker<MockResponseHttp> d("exec_create");
        std::string exec_id;
        ExecConfig config;
        DockerError e = d.execCreateInstance("containerId", config, exec_id);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(exec_id == "string");
    }

    TEST_CASE("Check creation of an exec instances handles error") {
        Docker<MockErrorHttp> d("404");
        std::string exec_id;
        ExecConfig config;
        DockerError e = d.execCreateInstance("containerId", config, exec_id);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check start an exec instances returns OK") {
        Docker<MockResponseHttp> d("200");
        DockerError e = d.execStartInstance("instance_id");
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }

    TEST_CASE("Check start an exec instances handles error") {
        Docker<MockErrorHttp> d("409");
        DockerError e = d.execStartInstance("instance_id");
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }

    TEST_CASE("Check resize an exec instances returns OK") {
        Docker<MockResponseHttp> d("200");
        DockerError e = d.execResizeInstance("instance_id", 10, 10);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
    }


    TEST_CASE("Check resize an exec instances handles error") {
        Docker<MockErrorHttp> d("404");
        DockerError e = d.execResizeInstance("instance_id", 10, 10);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }


    TEST_CASE("Check inspect an exec instance handles error") {
        Docker<MockResponseHttp> d("exec_inspect");
        ExecInfo r;
        DockerError e = d.execInspectInstance("instance_id", r);
        CHECK(e.isOk() == true);
        CHECK(e.isError() == false);
        CHECK(r.canRemove == false);
        CHECK(r.containerID == "b53ee82b53a40c7dca428523e34f741f3abc51d9f297a14ff874bf761b995126");
        CHECK(r.detachKeys == "");
        CHECK(r.exitCode == 2);
        CHECK(r.id == "f33bbfb39f5b142420f4759b2348913bd4a8d1a6d7fd56499cb41a1bb91d7b3b");
        CHECK(r.openStderr == true);
        CHECK(r.openStdin == true);
        CHECK(r.openStdout == true);
        CHECK(r.processConfig.arguments.size() > 0);
        CHECK(r.processConfig.arguments[0] == "-c");
        CHECK(r.processConfig.arguments[1] == "exit 2");
        CHECK(r.processConfig.entrypoint == "sh");
        CHECK(r.processConfig.privileged == false);
        CHECK(r.processConfig.tty == true);
        CHECK(r.processConfig.user == "1000");
        CHECK(r.running == false);
        CHECK(r.pid == 42000);
    }

    TEST_CASE("Check inspect an exec instance handles error") {
        Docker<MockErrorHttp> d("404");
        ExecInfo r;
        DockerError e = d.execInspectInstance("instance_id", r);
        CHECK(e.isOk() == false);
        CHECK(e.isError() == true);
    }
}