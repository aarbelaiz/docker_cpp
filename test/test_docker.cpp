#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "test_utils.h"

using namespace docker_cpp;

TEST_CASE("Not empty <string,string> map to string") {
    std::map<std::string, std::string> m = {{"foo", "bar"}, {"foo2", "bar2"}};
    std::string c  = _map_to_string(m);
    CHECK(c == "{\"foo\":\"bar\",\"foo2\":\"bar2\"}");
}

TEST_CASE("Single element <string,string> map to string") {
    std::map<std::string, std::string> m = {{"foo", "bar"}};
    std::string c  = _map_to_string(m);
    CHECK(c == "{\"foo\":\"bar\"}");
}

TEST_CASE("Empty <string,string> map to string") {
    std::map<std::string, std::string> m;
    std::string c  = _map_to_string(m);
    CHECK(c == "");
}

TEST_CASE("Construction of single query parameters") {
    std::string q = query_params(q_arg("foo", "bar"));
    CHECK(q == "?foo=bar");
}

TEST_CASE("Construction of multiple query parameters") {
    std::string q = query_params(q_arg("foo", "bar"), q_arg("lorem", 3), q_arg("ipsum", std::string("test")));
    CHECK(q == "?foo=bar&lorem=3&ipsum=test");
}

TEST_CASE("Construction of query parameters with empty string argument") {
    std::string q = query_params(q_arg("foo", std::string()), q_arg("lorem", 3));
    CHECK(q == "?lorem=3");
}

TEST_CASE("Construction of query parameters with empty string literal") {
    std::string q = query_params(q_arg("foo", ""), q_arg("lorem", 3));
    CHECK(q == "?lorem=3");
}

TEST_CASE("Construction of query parameters with < 0 value") {
    std::string q = query_params(q_arg("foo", "bar"), q_arg("lorem", -1));
    CHECK(q == "?foo=bar");
}