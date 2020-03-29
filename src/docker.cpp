#include "docker_cpp/docker.h"
#include "docker_cpp/docker_parse.h"

#include <iostream>
#include <sstream>

#include <asl/String.h>
#include <asl/Http.h>
#include <asl/JSON.h>

inline const char * const BoolToText(bool b) { return b ? "true" : "false"; }

namespace docker_cpp {

Docker::Docker(const std::string &uri)
{
	std::cout << uri << std::endl;
	_endpoint = uri + "/v1.40";
}

Docker::Docker(const std::string &ip, const unsigned int port)
{
	Docker("http://"+ip+std::to_string(port));
}

//////////////////// SYSTEM //////////////////////////

DockerError Docker::version(VersionInfo &result)
{
	const std::string url = _endpoint + "/version/json";
	auto res = asl::Http::get(asl::String(url.c_str()));
	DockerError err = _checkError(&res);
	if (!err.isOk()) return err;
	auto data = res.json();
	parseVersionInfo(&data, result);
	return err;
}

DockerError Docker::ping()
{
	const std::string url = _endpoint + "/_ping";
	auto res = asl::Http::get(asl::String(url.c_str()));
	return _checkError(&res);
}

//////////////////// IMAGES //////////////////////////

DockerError Docker::images(imageList &result, bool all, bool digests)
{
	std::string url = _endpoint + "/images/json";
	url += concat_query_args(q_arg("all", all), q_arg("digests", digests));
	auto res = asl::Http::get(asl::String(url.c_str()));
	DockerError err = _checkError(&res);
	if (!err.isOk()) return err;
	auto data = res.json();
	parseImages(&data, result);
	return err;
}

DockerError Docker::tagImage(const std::string &name, const std::string &repo, const std::string &tag)
{
	std::string url = _endpoint + "/images/" + name + "/tag/json";
	if (!repo.empty()) url += "?repo=" + repo;
	if (!tag.empty()) url += "?tag=" + tag;
	auto res = asl::Http::post(asl::String(url.c_str()), "");
	return _checkError(&res);
}

DockerError Docker::removeImage(const std::string &name, bool force, bool noprune)
{
	std::string url = _endpoint + "/images/" + name + "/json?force=" + BoolToText(force) + "?noprune=" + BoolToText(noprune);
	auto res = asl::Http::delet(asl::String(url.c_str()));
	return _checkError(&res);
}

//////////////////// CONTAINERS //////////////////////////

DockerError Docker::containers(containerList &result, bool all, int limit, bool size)
{
	std::string url = _endpoint + "/containers/json?all=" + BoolToText(all) + "?size=" + BoolToText(size);
	if (limit > 0) url += "?limit=" + std::to_string(limit);
	auto res = asl::Http::get(asl::String(url.c_str()));
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
	auto res = asl::Http::post(asl::String(url.c_str()), "");
	return _checkError(&res);
}

DockerError Docker::stopContainer(const std::string &id, int t)
{
	std::string url = _endpoint + "/containers/" + id + "/stop/json";
	if (t > 0) url += "?t=" + std::to_string(t);
	auto res = asl::Http::post(asl::String(url.c_str()), "");
	return _checkError(&res);
}

DockerError Docker::restartContainer(const std::string &id, int t)
{
	std::string url = _endpoint + "/containers/" + id + "/restart/json";
	if (t > 0) url += "?t=" + std::to_string(t);
	auto res = asl::Http::post(asl::String(url.c_str()), "");
	return _checkError(&res);
}

DockerError Docker::killContainer(const std::string &id, const std::string &signal)
{
	const std::string url = _endpoint + "/containers/" + id + "/kill/json?signal=" + signal;
	auto res = asl::Http::post(asl::String(url.c_str()), "");
	return _checkError(&res);
}

DockerError Docker::renameContainer(const std::string &id, const std::string &name)
{
	const std::string url = _endpoint + "/containers/" + id + "/rename/json?name=" + name;
	auto res = asl::Http::post(asl::String(url.c_str()), "");
	return _checkError(&res);
}

DockerError Docker::waitContainer(const std::string &id, WaitInfo &result, const std::string &condition)
{
	const std::string url = _endpoint + "/containers/" + id + "/wait/json?condition=" + condition;
	auto res = asl::Http::post(asl::String(url.c_str()), "");
	DockerError err = _checkError(&res);
	if (err.isError()) return err;
	auto data = res.json();
	parseWaitInfo(&data, result);
	return err;
}

DockerError Docker::removeContainer(const std::string &id, bool v, bool force, bool link)
{
	const std::string url = _endpoint + "/containers/" + id + "/json?v=" + BoolToText(v) + "?force=" + BoolToText(force) + "?link=" + BoolToText(link);
	auto res = asl::Http::delet(asl::String(url.c_str()));
	return _checkError(&res);
}


DockerError Docker::createExecInstance(const std::string &id, ExecConfig &config, std::string &execId)
{
	const std::string url = _endpoint + "/containers/" + id + "/exec";
	// TODO: create body
	auto res = asl::Http::post(asl::String(url.c_str()), "body");
	DockerError err = _checkError(&res);
	if (err.isError()) return err;
	auto data = res.json();
	execId = *data["Id"].toString();
	return err;
}

DockerError Docker::startExecInstance(const std::string &id, bool detach, bool tty)
{
	const std::string url = _endpoint + "/exec/" + id + "/start";
	std::stringstream ss;
	ss << "{\"detach\":\"" << BoolToText(detach) << "\",\"tty\":" << BoolToText(tty) << "\"}";
	std::string body = ss.str();
	auto res = asl::Http::post(asl::String(url.c_str()), body.c_str());
	return _checkError(&res);
}

DockerError Docker::resizeExecInstance(const std::string &id, int h, int w)
{
	const std::string url = _endpoint + "/exec/" + id + "/resize?h=" + std::to_string(h) + "?w=" + std::to_string(w);
	auto res = asl::Http::post(asl::String(url.c_str()), "");
	return _checkError(&res);
}

DockerError Docker::inspectInstance(const std::string &id, ExecInfo &result)
{
	const std::string url = _endpoint + "/exec/" + id + "/json";
	auto res = asl::Http::get(asl::String(url.c_str()));
	DockerError err = _checkError(&res);
	if (err.isError()) return err;
	asl::Var data = asl::Json::decode(res.text().replace("\\\"", "")); // AAA: scaping is necessary for commands
	parseExec(&data, result);
	return err;
}

bool Docker::checkConnection()
{
	auto r = this->ping();
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
