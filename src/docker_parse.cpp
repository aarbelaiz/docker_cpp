#include <docker_cpp/docker_parse.h>

#include <asl/String.h>
#include <asl/Var.h>
#include <iostream>

#include <utility>
namespace docker_cpp
{

	void parse(const asl::Var &in, ImageList &out)
	{
		foreach (asl::Var &image, in)
		{
			ImageInfo info;
			info.id = *image["Id"].toString();
			info.parentId = *image["ParentId"].toString();
			foreach (asl::Var &tag, image["RepoTags"])
			{
				info.repoTags.push_back(*tag.toString());
			}
			foreach (asl::Var &repo, image["RepoDigests"])
			{
				info.repoDigests.push_back(*repo.toString());
			}
			info.created = image["Created"];
			info.size = image["Size"];
			info.virtualSize = image["VirtualSize"];
			info.sharedSize = image["SharedSize"];
			if (image.has("Labels"))
			{
				for (auto &l : image["Labels"].object())
				{
					auto entry = std::make_pair(*l.key, *l.value.toString());
					info.labels.push_back(entry);
				}
			}
			info.containers = image["Containers"];
			out.push_back(info);
		}
	}

	void parse(const asl::Var &in, DeletedImageList &out)
	{
		foreach(asl::Var &i, in) {
			DeletedImageInfo d_info;
			if (i.has("Untagged")) d_info.untagged = *(i["Untagged"].toString());
			if (i.has("Deleted")) d_info.deleted = *(i["Deleted"].toString());
			out.push_back(d_info);
		}
	}

	void parse(const asl::Var &in, PruneInfo &out)
	{
		if (in.has("ImagesDeleted")) {
			parse(in["ImagesDeleted"], out.imagesDeleted);
		}
		out.spaceReclaimed = (asl::ULong)in["SpaceReclamed"];
	}

	void parse(const asl::Var &in, ContainerList &out)
	{
		foreach (asl::Var &container, in)
		{
			ContainerInfo info;
			info.id = *(container["Id"].toString());
			foreach (auto name, container["Names"])
			{
				info.names.push_back(*name.toString());
			}
			info.image = *container["Image"].toString();
			info.imageID = *container["ImageID"].toString();
			info.command = *container["Command"].toString();
			info.created = static_cast<asl::Long>(container["Created"]);
			if (container.has("Ports"))
			{
				foreach (auto& portInfo, container["Ports"])
				{
					Port port;
					parse(portInfo, port);
					info.ports.push_back(port);
				}
			}
			info.sizeRw = static_cast<asl::Long>(container["SizeRw"]);
			info.sizeRootFs = static_cast<asl::Long>(container["SizeRootFs"]);
			if (container.has("Labels"))
			{
				for (auto &l : container["Labels"].object())
				{
					auto entry = std::make_pair(*l.key, *l.value.toString());
					info.labels.push_back(entry);
				}
			}
			info.state = *container["State"].toString();
			info.status = *container["Status"].toString();
			if (container.has("HostConfig"))
			{
				for (auto &c : container["HostConfig"].object())
				{
					info.hostConfig = std::make_pair(*c.key, *c.value.toString());
				}
			}
			if (container.has("NetworkSettings"))
			{
				NetworkSettings networkSettings;
				parse(container["NetworkSettings"], networkSettings);
				info.networkSettings = networkSettings;
			}
			out.push_back(info);
		}
	}

	void parse(const asl::Var &in, Port &out)
	{
		out.ip = *(in["Ip"].toString());
		out.privatePort = in["PrivatePort"];
		out.publicPort = in["PublicPort"];
		out.type = *(in["Type"].toString());
	}

	void parse(const asl::Var &in, NetworkSettings &out)
	{
		if (in.has("Networks"))
		{		 
			for(auto& n : in["Networks"].object()) 
			{
				EndpointSettings endpoint;
				foreach (auto link, n.value["Links"])
				{
					endpoint.links.push_back(*link.toString());
				}
				foreach (auto alias, n.value["Aliases"])
				{
					endpoint.links.push_back(*alias.toString());
				}
				endpoint.networkID = *n.value["NetworkID"].toString();
				endpoint.endpointID = *n.value["EndpointID"].toString();
				endpoint.gateway = *n.value["Gateway"].toString();
				endpoint.ipAddress = *n.value["IPAddress"].toString();
				endpoint.ipPrefixLen = n.value["IPPrefixLen"];
				endpoint.ipV6Gateway = *n.value["IPv6Gateway"].toString();
				endpoint.globalIpV6Address = *n.value["GlobalIPv6Address"].toString();
				endpoint.globalIpV6PrefixLen = (int)n.value["GlobalIPv6PrefixLen"];
				endpoint.macAddress = *n.value["MacAddress"].toString();
				out.networks.emplace_back(*n.key, std::move(endpoint));
			}
		}
	}

	void parse(const asl::Var &in, VersionInfo &out)
	{
		if (in.has("Components"))
		{
			foreach (asl::Var &component, in["Components"])
			{
				Component comp;
				comp.name = *component["Name"].toString();
				comp.version = *component["Version"].toString();
				out.components.push_back(comp);
			}
		}
		out.version = *(in["Version"].toString());
		out.apiVersion = *(in["ApiVersion"].toString());
		out.minApiVersion = *(in["MinAPIVersion"].toString());
		out.gitCommit = *(in["GitCommit"].toString());
		out.os = *(in["Os"].toString());
		out.arch = *(in["Arch"].toString());
		out.kernelVersion = *(in["KernelVersion"].toString());
		out.experimental = in["Experimental"];
		out.buildTime = *(in["BuildTime"].toString());
		out.goVersion = *(in["GoVersion"].toString());
	}

	void parse(const asl::Var &in, WaitInfo &out)
	{
		out.statusCode = in["StatusCode"];
		if (in.has("Error"))
		{
			out.errorMsg = *(in["Error"]["Message"].toString());
		}
	}

	void parse(const asl::Var &in, ExecInfo &out)
	{
		out.canRemove = in["CanRemove"];
		out.detachKeys = *(in["DetachKeys"].toString());
		out.exitCode = in["ExitCode"];
		out.id = *(in["ID"].toString());
		out.running = in["Running"];
		out.openStdin = in["OpenStdin"];
		out.openStderr = in["OpenStderr"];
		out.openStdout = in["OpenStdout"];
		out.containerID = *(in["ContainerID"].toString());
		out.pid = in["Pid"];
		if (in.has("ProcessConfig"))
		{
			asl::Var p = in["ProcessConfig"];
			out.processConfig.privileged = p["privileged"];
			out.processConfig.user = *(p["user"].toString());
			out.processConfig.tty = p["tty"];
			out.processConfig.entrypoint = *(p["entrypoint"].toString());
			foreach (asl::Var &arg, p["arguments"])
			{
				out.processConfig.arguments.push_back(*arg.toString());
			}
		}
	}

    void parse(const asl::Var &in, VolumeList &out)
	{
		for(auto &v : in["Volumes"]) {
			VolumeInfo i;
			parse(v, i);
			out.volumes.push_back(std::move(i));
		}
		for(auto &w : in["Warnings"]) {
			out.warnings.emplace_back(*w.toString());
		}
	}

    void parse(const asl::Var &in, VolumeInfo &out)
	{
		out.name = *(in["Name"].toString());
		out.driver = *(in["Driver"].toString());
		out.mountpoint = *(in["Mountpoint"].toString());
		if (in.has("CreatedAt")) out.createdAt = *(in["CreatedAt"].toString());
		if (in.has("Status")) {
			for (auto &l : in["Status"].object()) {
				out.status[*l.key] = *l.value.toString();
			}
		}
		for (auto &l : in["Labels"].object()) {
			out.labels[*l.key] = *l.value.toString();
		}
		out.scope = *(in["Scope"].toString());
		for (auto &l : in["Options"].object()) {
			out.driverOptions[*l.key] = *l.value.toString();
		}
		if (in.has("UsageData")) {
			out.usageData.reset(new VolumeUsageData);
			out.usageData->size = in["UsageData"]["Size"];
			out.usageData->refCount = in["UsageData"]["RefCount"];
		}
	}

	void parse(const asl::Var &in, DeletedVolumesInfo &out)
	{
		if (in.has("VolumesDeleted"))
		{
			for(auto &v: in["VolumesDeleted"]) {
				out.volumesDeleted.push_back(*(v.toString()));
			}
		}
		out.spaceReclaimed = static_cast<asl::Long>(in["SpaceReclaimed"]);
	}
} // namespace docker_cpp
