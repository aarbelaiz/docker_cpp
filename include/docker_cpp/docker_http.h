#ifndef _DOCKER_HTTPSERVER_H
#define _DOCKER_HTTPSERVER_H

#include <asl/String.h>
#include <asl/Http.h>

#include <string>
#include <sstream>
#include <map>

#include <cstring>

namespace docker_cpp
{
	template<typename T>
	inline bool query_param_is_empty(const std::pair<std::string, T>& value)
	{ return false; }

	inline bool query_param_is_empty(const std::pair<std::string, int>& value)
	{ return value.second < 0; }

	inline bool query_param_is_empty(const std::pair<std::string, std::string>& value)
	{ return value.second.empty(); }

	inline bool query_param_is_empty(const std::pair<std::string, const char*>& value)
	{ return strlen(value.second) == 0; }

	template<typename T>
	void acc_query_params(std::string& s, const std::pair<std::string, T>& value)
	{
		std::string t;
		if (!query_param_is_empty(value)) {
			std::ostringstream oss;
			oss << std::boolalpha << value.first << "=" << value.second;
			t = std::move(oss.str());
		}
		s = s + (s.empty() || t.empty() ? "" : "&" ) + t;
	}

	template<typename T, typename ...Args>
	void acc_query_params(std::string& s, const std::pair<std::string, T>& value, const Args& ...args)
	{
		acc_query_params(s, value);
		acc_query_params(s, args...);
	}

	template<typename T, typename ...Args>
	std::string query_params(const std::pair<std::string, T>& value, const Args& ...args)
	{
		std::string params_uri;
		acc_query_params(params_uri, value, args...);
		return !params_uri.empty() ? "?" + params_uri : std::string();
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
			asl::Dic<> asl_hs;
			for (auto &h: headers) { asl_hs = asl_hs(asl::String(h.first.c_str()), asl::String(h.second.c_str())); }
			//std::cout << *body << std::endl;
			return asl::Http::post(asl::String(uri.c_str()), body, asl_hs);
		};

		asl::HttpResponse postImpl(const std::string &uri, const std::string &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			return postImpl(uri, asl::String(body.c_str()), headers);
		};

		template <typename T>
		asl::HttpResponse putImpl(const std::string &uri, const T &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			asl::Dic<> asl_hs;
			for (auto &h: headers) { asl_hs = asl_hs(asl::String(h.first.c_str()), asl::String(h.second.c_str())); }
			return asl::Http::put(asl::String(uri.c_str()), body);
		};

		asl::HttpResponse putImpl(const std::string &uri, const std::string &body, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{ 
			return putImpl(uri, asl::String(body.c_str()), headers);
		};

		asl::HttpResponse deletImpl(const std::string &uri, const std::map<std::string, std::string> &headers = std::map<std::string, std::string>())
		{
			return asl::Http::delet(asl::String(uri.c_str()));
		};
	};

	typedef ASLHttp asl_interface;
} // namespace docker_cpp
#endif