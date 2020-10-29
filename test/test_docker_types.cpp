#include <doctest/doctest.h>
#include <docker_cpp/docker_types.h>
#include <docker_cpp/docker_utils.h>

using namespace docker_cpp;

TEST_SUITE("TYPES") {
    TEST_CASE("Check serialization") {
        std::cout << std::boolalpha;
        std::string str = "i am a string";
        std::cout << is_string<decltype(str)>::value << std::endl; // "true"
        std::cout << is_string<decltype("i am a string literal")>::value << std::endl;
        std::cout << to_json("hsy", 2) << std::endl;
        ImageInfo info;
        std::cout << info.json() << std::endl;
        CHECK(!is_string<int>::value);
        CHECK(is_string<char *>::value);
        CHECK(is_string<char const *>::value);
        CHECK(is_string<char *const>::value);
        CHECK(is_string<char const *const>::value);
        CHECK(is_string<char(&)[5]>::value);
        CHECK(is_string<char const(&)[5]>::value);
    }
}