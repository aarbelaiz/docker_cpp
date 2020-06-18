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
