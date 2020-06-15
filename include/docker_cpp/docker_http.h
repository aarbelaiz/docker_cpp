#ifndef _DOCKER_HTTPSERVER_H
#define _DOCKER_HTTPSERVER_H

#include <string>
#include <map>

#include <asl/String.h>
#include <asl/Http.h>

#include <type_traits>

namespace docker_cpp
{
	template<typename T>
	std::string query_params(const std::pair<std::string, T>& value)
	{
		std::ostringstream oss;
		oss << std::boolalpha << "?" << value.first << "=" << value.second;
		return oss.str();
	}

	std::string query_params(const std::pair<std::string, int>& value)
	{
		if (value.second >= 0) {
			std::ostringstream oss;
			oss << "?" << value.first << "=" << value.second;
			return oss.str();
		}else{
			return std::string();
		}
	}

	std::string query_params(const std::pair<std::string, char>& value)
	{
		if (value.second != '\0') {
			std::ostringstream oss;
			oss << "?" << value.first << "=" << value.second;
			return oss.str();
		}else{
			return std::string();
		}
	}

	std::string query_params(const std::pair<std::string, std::string>& value)
	{
		if (!value.second.empty()) {
			std::ostringstream oss;
			oss << std::boolalpha << "?" << value.first << "=" << value.second;
			return oss.str();
		}else{
			return std::string();
		}
	}

	template<typename T, typename ...Args>
	std::string query_params(const std::pair<std::string, T>& value, const Args& ...args)
	{
		return query_params(value) + query_params(args...);
	}

	template<typename T>
	std::pair<std::string, T> q_arg(const std::string& fst, const T scd) 
	{ 
		return std::make_pair(std::move(fst), std::move(scd));
	}

	template <typename Derived>
	struct DockerHttpInterface
	{
		asl::HttpResponse get(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			return static_cast<Derived*>(this)->getImpl(uri, headers);
		}

		template <typename T>
		asl::HttpResponse post(const std::string &uri, const T &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			return static_cast<Derived*>(this)->postImpl(uri, body, headers);
		}

		template <typename T>
		asl::HttpResponse put(const std::string &uri, const T &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			return static_cast<Derived*>(this)->putImpl(uri, body, headers);
		}

		asl::HttpResponse delet(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			return static_cast<Derived*>(this)->deletImpl(uri, headers);
		}
	};

	struct ASLHttp : DockerHttpInterface<ASLHttp>
	{
		asl::HttpResponse getImpl(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			return asl::Http::get(asl::String(uri.c_str()));
		};
		
		template <typename T>
		asl::HttpResponse postImpl(const std::string &uri, const T &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{ 
			return asl::Http::post(asl::String(uri.c_str()), body);
		};

		template <typename T>
		asl::HttpResponse putImpl(const std::string &uri, const T &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{ 
			return asl::Http::put(asl::String(uri.c_str()), body);
		};

		asl::HttpResponse deletImpl(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			return asl::Http::delet(asl::String(uri.c_str()));
		};
	};

	typedef ASLHttp asl_interface;
} // namespace docker_cpp
#endif