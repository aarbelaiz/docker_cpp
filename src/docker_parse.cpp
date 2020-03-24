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
	if (in->has("Networks")) {
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

void parseVersionInfo(asl::Var *in, VersionInfo &out)
{
    if (in->has("Components")){
        foreach(asl::Var& component, (*in)["Components"]) {
            Component comp;
            comp.name = *component["Name"].toString();
            comp.version = *component["Version"].toString();
            out.components.push_back(comp);
        }
    }
    out.version = *((*in)["Version"].toString());
    out.apiVersion = *((*in)["ApiVersion"].toString());
    out.minApiVersion = *((*in)["MinApiVersion"].toString());
    out.gitCommit = *((*in)["GitCommit"].toString());
    out.os = *((*in)["Os"].toString());
    out.arch = *((*in)["Arch"].toString());
    out.kernelVersion = *((*in)["KernelVersion"].toString());
    out.experimental = (*in)["Experimental"];
    out.buildTime = *((*in)["BuildTime"].toString());
}

void parseWaitInfo(asl::Var *in, WaitInfo &out)
{
	out.statusCode = (*in)["StatusCode"];
	if (in->has("Error")){
		out.errorMsg = *((*in)["Error"]["Message"].toString());
	}
}

void parseExec(asl::Var *in, ExecInfo &out)
{
	out.canRemove = (*in)["CanRemove"];
	out.detachKeys = *((*in)["DetachKeys"].toString());
	out.id = *((*in)["Id"].toString());
	out.running = (*in)["Running"];
	if (in->has("ProcessConfig")) {
		ProcessConfig process;
		asl::Var p = (*in)["ProcessConfig"];
		process.privileged = p["privileged"];
		process.user = *p["user"].toString();
		process.tty = p["tty"];
		process.entrypoint = *p["entrypoint"].toString();
		foreach(asl::Var &arg, p["arguments"])
		{
			process.arguments.push_back(*arg.toString());
		}
		out.processConfig = process;
	}
	out.openStdin = (*in)["OpenStdin"];
	out.openStderr = (*in)["OpenStderr"];
	out.openStdout = (*in)["OpenStdout"];
	out.containerID = *((*in)["ContainerID"].toString());
	out.pid = (*in)["Pid"];
}

}
