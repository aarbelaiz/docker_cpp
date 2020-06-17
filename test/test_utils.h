#ifndef __TEST_UTILS_H_
#define __TEST_UTILS_H_

#include "test_config.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <docker_cpp/docker.h>

#include <asl/File.h>

#include <iostream>

namespace docker_cpp
{
    struct MockResponseHttp : DockerHttpInterface<MockResponseHttp>
    {
        asl::HttpResponse _fromFile(const std::string& uri, const std::string& method) { 
            asl::String filename = asl::String(uri.c_str()).split("/v1.")[0];
            asl::String filepath = asl::String(TEST_RESPONSES_PATH) + "/" + filename + "_get.json";
            asl::Array<byte> data = asl::File(filepath, asl::File::READ).content();
            asl::HttpResponse r = asl::HttpResponse();
            r.put(data);
            r.setCode(200);
            return r;
        }

        asl::HttpResponse getImpl(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
        {
            return _fromFile(uri, "get");
        }

		template <typename T>
		asl::HttpResponse postImpl(const std::string &uri, const T &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{ 
			return _fromFile(uri, "post");
		};

		template <typename T>
		asl::HttpResponse putImpl(const std::string &uri, const T &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{ 
			return _fromFile(uri, "put");
		};

		asl::HttpResponse deletImpl(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			return _fromFile(uri, "delet");
		};
    };

    struct MockErrorHttp : DockerHttpInterface<MockErrorHttp>
    {
        asl::HttpResponse getImpl(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
        {
            auto r = asl::HttpResponse();
            r.setCode(500);
            return r;
        }

		template <typename T>
		asl::HttpResponse postImpl(const std::string &uri, const T &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{ 
			auto r = asl::HttpResponse();
            r.setCode(500);
            return r;
		};

		template <typename T>
		asl::HttpResponse putImpl(const std::string &uri, const T &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{ 
			auto r = asl::HttpResponse();
            r.setCode(500);
            return r;
		};

		asl::HttpResponse deletImpl(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			auto r = asl::HttpResponse();
            r.setCode(500);
            return r;
		};
    };
}

#endif // __TEST_UTILS_H_
