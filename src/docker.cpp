#include <docker_cpp/docker.h>

#include <iostream>

#include <asl/String.h>
#include <asl/Http.h>

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

dockErr Docker::images(imageList &result, bool all, bool digests)
{
	const std::string url = _endpoint + "/images/json?all=" + BoolToText(all) + "?digests=" + BoolToText(digests);
	auto res = Http::get(asl::String(url.c_str()));
	if (!res.ok()){
		std::cout << res.code() << std::endl;
		return DOCKER_SERVER_ERROR;
	}
	auto data = res.json();
	_parseImages(&data, result);
	return DOCKER_OK;
}

void Docker::_parseImages(Var *in, imageList &out)
{
	foreach(Var& image, *in)
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
		for(auto& l : image["Labels"].object())
		{
			auto entry = std::make_pair<std::string, std::string>(*l.key, *l.value.toString());
			info.labels.push_back(entry);
		}
		info.containers = image["Containers"];
		out.push_back(info);
	}
}

}
