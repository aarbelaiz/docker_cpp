#include "docker_cpp/docker_parse.h"

#include <asl/String.h>
#include <asl/Var.h>

namespace docker_cpp {

void parseImages(asl::Var *in, imageList &out)
{
	foreach(asl::Var& image, *in)
	{
		ImageInfo info;
		info.id = *image["Id"].toString();
		info.parentId = *image["ParentId"].toString();
		foreach(asl::Var& tag, image["RepoTags"])
		{
			info.repoTags.push_back(*tag.toString());
		}
		foreach(asl::Var& repo, image["RepoDigests"])
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

void parseContainers(asl::Var *in, containerList &out)
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
				parsePort(&portInfo, port);
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
			parseNetwork(&container["NetworkSettings"], networkSettings);
			info.networkSettings = networkSettings;
		}
		out.push_back(info);
	}
}

void parsePort(asl::Var *in, Port &out)
{
	out.ip = *((*in)["Ip"].toString());
	out.privatePort = *((*in)["PrivatePort"].toString());
	out.publicPort = *((*in)["PublicPort"].toString());
	out.type = *((*in)["Type"].toString());
}

void parseNetwork(asl::Var *in, NetworkSettings &out)
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
