#include "docker_cpp/docker.h"
#include "docker_cpp/docker_parse.h"

#include <iostream>

#include <asl/String.h>
#include <asl/Http.h>
#include <asl/JSON.h>

inline const char * const BoolToText(bool b)
{
  return b ? "true" : "false";
}

using namespace asl;

namespace docker_cpp {

Docker::Docker(const std::string &uri)
{
	std::cout << uri << std::endl;
	_endpoint = uri;
}

Docker::Docker(const std::string &ip, const unsigned int port)
{
	Docker("http://"+ip+std::to_string(port));
}

DockerError Docker::info()
{
	return DockerError::OK();
}

DockerError Docker::images(imageList &result, bool all, bool digests)
{
	const std::string url = _endpoint + "/images/json?all=" + BoolToText(all) + "?digests=" + BoolToText(digests);
	auto res = Http::get(asl::String(url.c_str()));
	DockerError err = _checkError(&res);
	if (!err.isOk()) return err;
	auto data = res.json();
	parseImages(&data, result);
	return err;
}

DockerError Docker::containers(containerList &result, bool all, int limit, bool size)
{
	std::string url = _endpoint + "/containers/json?all=" + BoolToText(all) + "?size=" + BoolToText(size);
	if (limit > 0) url += url + "?limit=" + std::to_string(limit);
	auto res = Http::get(asl::String(url.c_str()));
	DockerError err = _checkError(&res);
	if (!err.isOk()) return err;
	asl::String filteredResponse = res.text().replace("\\\"", ""); // AAA: scaping is necessary for commands
	asl::Var data = asl::Json::decode(filteredResponse);
	parseContainers(&data, result);
	return err;
}

DockerError Docker::startContainer(const std::string &id, const std::string &detachKeys)
{
	const std::string url = _endpoint + "/containers/" + id + "/start/json?detachKeys=" + detachKeys;
	auto res = Http::get(asl::String(url.c_str()));
	return _checkError(&res);
}

DockerError Docker::stopContainer(const std::string &id, int t)
{
	const std::string url = _endpoint + "/containers/" + id + "/stop/json?t=" + std::to_string(t);
	auto res = Http::get(asl::String(url.c_str()));
	return _checkError(&res);
}

DockerError Docker::restartContainer(const std::string &id, int t)
{
	const std::string url = _endpoint + "/containers/" + id + "/restart/json?t=" + std::to_string(t);
	auto res = Http::get(asl::String(url.c_str()));
	return _checkError(&res);
}

DockerError Docker::killContainer(const std::string &id, const std::string &signal)
{
	const std::string url = _endpoint + "/containers/run/" + id + "/kill/json?signal=" + signal;
	auto res = Http::get(asl::String(url.c_str()));
	return _checkError(&res);
}

bool Docker::checkConnection()
{
	auto r = this->info();
	return r.isOk();
}

DockerError Docker::_checkError(asl::HttpResponse *res)
{
	int code = res->code();
	if (code == 200 || code == 204) {
		return DockerError::OK();
	} else if (code < 400) {
		std::string msg = *(res->json()["message"].toString());
		return DockerError::INFO(msg, code); 
	}else{
		std::string msg = *(res->json()["message"].toString());
		return DockerError::ERROR(msg, code);
	}	
}

}
