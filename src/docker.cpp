#include <docker_cpp/docker.h>

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

DockerError Docker::images(imageList &result, bool all, bool digests)
{
	const std::string url = _endpoint + "/images/json?all=" + BoolToText(all) + "?digests=" + BoolToText(digests);
	auto res = Http::get(asl::String(url.c_str()));
	if (!res.ok()){
		return DockerError(DOCKER_ERROR, *res.json()["message"].toString(), res.code());
	}
	auto data = res.json();
	_parseImages(&data, result);
	return DockerError(DOCKER_OK, "", 200);
}

DockerError Docker::containers(containerList &result, bool all, int limit, bool size)
{
	std::string url = _endpoint + "/containers/json?all=" + BoolToText(all) + "?size=" + BoolToText(size);
	if (limit > 0) url += url + "?limit=" + std::to_string(limit);
	auto res = Http::get(asl::String(url.c_str()));
	if (!res.ok()){
		return DockerError(DOCKER_ERROR, *res.json()["message"].toString(), res.code());
	}
	asl::String filteredResponse = res.text().replace("\\\"", ""); // AAA: scaping is necessary for commands
	asl::Var data = asl::Json::decode(filteredResponse);
	_parseContainers(&data, result);
	return DockerError(DOCKER_OK, "", 200);
}

DockerError Docker::runContainer(const std::string &id, const std::string &detachKeys)
{
	const std::string url = _endpoint + "/containers/run/" + id + "/json?detachKeys=" + detachKeys;
	auto res = Http::get(asl::String(url.c_str()));
	if (!res.ok()) return DockerError(DOCKER_ERROR, *res.json()["message"].toString(), res.code());
	return DockerError(DOCKER_OK, "", 200);
}


void Docker::_parseImages(Var *in, imageList &out)
{
	foreach(asl::Var& image, *in)
	{
		ImageInfo info;
		info.id = *image["Id"].toString();
		info.parentId = *image["ParentId"].toString();
		foreach(Var& tag, image["RepoTags"])
		{
			info.repoTags.push_back(*tag.toString());
		}
		foreach(Var& repo, image["RepoDigests"])
		{
			info.repoDigests.push_back(*repo.toString());
		}
		info.created = image["Created"];
		info.size = image["Size"];
		info.virtualSize = image["VirtualSize"];
		info.sharedSize = image["SharedSize"];
		if (image.has("Labels")) {
			for(auto& l : image["Labels"].object())
			{
				auto entry = std::make_pair<std::string, std::string>(*l.key, *l.value.toString());
				info.labels.push_back(entry);
			}
		}
		info.containers = image["Containers"];
		out.push_back(info);
	}
}

void Docker::_parseContainers(Var *in, containerList &out)
{
	foreach(asl::Var &container, *in) {
		ContainerInfo info;
		info.id = *container["Id"].toString();
		foreach(auto name, container["Names"]) {
			info.names.push_back(*name.toString());
		}
		info.image = *container["Image"].toString();
		info.imageID = *container["ImageID"].toString();
		info.command = *container["Command"].toString();
		info.created = static_cast<asl::Long>(container["created"]);
		if (container.has("Ports")) {
			foreach(auto portInfo, container["Ports"]) {
				Port port;
				_parsePort(&portInfo, port);
				info.ports.push_back(port);	
			}
		}
		info.sizeRw = static_cast<asl::Long>(container["SizeRw"]);
		info.sizeRootFs = static_cast<asl::Long>(container["SizeRootFs"]);
		if (container.has("Labels")) {
			for(auto& l : container["Labels"].object())
			{
				auto entry = std::make_pair(*l.key, *l.value.toString());
				info.labels.push_back(entry);
			}
		}
		info.state = *container["State"].toString();
		info.status = *container["Status"].toString();
		if (container.has("HostConfig")) {
			for(auto& c : container["HostConfig"].object()){
				info.hostConfig = std::make_pair(*c.key, *c.value.toString());
			}
		}
		if (container.has("NetworkSettings")) {
			NetworkSettings networkSettings;
			_parseNetwork(&container["NetworkSettings"], networkSettings);
			info.networkSettings = networkSettings;
		}
		out.push_back(info);
	}
}

void Docker::_parsePort(asl::Var *in, Port &out)
{
	out.ip = *((*in)["Ip"].toString());
	out.privatePort = *((*in)["PrivatePort"].toString());
	out.publicPort = *((*in)["PublicPort"].toString());
	out.type = *((*in)["Type"].toString());
}

void Docker::_parseNetwork(asl::Var *in, NetworkSettings &out)
{
	if (in->has("networks")) {
		foreach(auto network, (*in)["Networks"]) {
			EndpointSettings endpoint;
			foreach(auto link, network["Links"]) {
				endpoint.links.push_back(*link.toString());
			}
			foreach(auto alias, network["Aliases"]) {
				endpoint.links.push_back(*alias.toString());
			}
			endpoint.networkID = *network["NetworkID"].toString();
			endpoint.endpointID = *network["EndpointID"].toString();
			endpoint.gateway = *network["Gateway"];
			endpoint.ipAddress = *network["IpAddress"].toString();
			endpoint.ipPrefixLen = network["IpPrefixLen"];
			out.networks.push_back(std::make_pair(*network.toString(), endpoint));
		}
	}
}


}
