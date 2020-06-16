#ifndef __TEST_UTILS_H_
#define __TEST_UTILS_H_

#include <docker_cpp/docker_http.h>

namespace docker_cpp
{
    struct MockInfoHttp : DockerHttpInterface<MockInfoHttp>
    {
        asl::HttpResponse getImpl(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
        {
            auto r = asl::HttpResponse();
            r.setCode(200);
            return r;
            //return asl::Http::get(asl::String(uri.c_str()));
        }
    };

    struct MockErrorHttp : DockerHttpInterface<MockErrorHttp>
    {
        asl::HttpResponse getImpl(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
        {
            auto r = asl::HttpResponse();
            r.setCode(500);
            return r;
            //return asl::Http::get(asl::String(uri.c_str()));
        }
    };
}

#endif // __TEST_UTILS_H_
