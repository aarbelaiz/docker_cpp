#include "docker_cpp/docker.h"

#include <iostream>
#include <iomanip>

using namespace docker_cpp;

int main()
{
	Docker<ASLHttp> docker("http://127.0.0.1:2375");

	// Check if there is a connection with docker daemon
	bool connected = docker.checkConnection();
	if (!connected) {
		std::cout << "Could not connect to docker engine!\n";
		return EXIT_FAILURE;
	}else{
		std::cout << "Connection successfully established...\n";
	}

	// Get system info...
	VersionInfo vInfo;
	DockerError err = docker.version(vInfo);
	if (err.isOk()) {
		std::cout << "api:" << vInfo.apiVersion << '\n';
		std::cout << "os:" << vInfo.os << '\n';
	}else{
		std::cout << "[VERSION]" << err << '\n';
	}


	// List images
	ImageList res;
	err = docker.images(res);
	if (err.isOk()) {
		std::cout << "--- IMAGES ----" << '\n';
		std::cout << std::left << std::setw(75) << "IMAGE ID" << std::left << std::setw(35) << "REPO" 
				  << std::left << std::setw(35) << "LABEL" << std::left << std::setw(15) << "CREATED"
				  << std::left << std::setw(15) << "SIZE" << '\n';
		for(auto &img : res) {
			std::string repos = (!img.repoTags.empty()) ? img.repoTags[0] : "<none>";
			std::string labels = (!img.labels.empty()) ? img.labels[0].first + ":" + img.labels[0].second : "<none>";
			std::cout << std::left << std::setw(75) << img.id << std::left << std::setw(35) << repos
					  << std::left << std::setw(35) << labels << std::left << std::setw(15) << img.created
					  << std::left << std::setw(15) << img.size << '\n';
		}
		std::cout << "---------------" << '\n';
	}

	// List containers
	ContainerList containers;
	err = docker.containers(containers, true);
	if (err.isError()) {
		std::cout << err << '\n';
		return EXIT_FAILURE;
	}
	std::cout << "--- CONTAINERS ----" << '\n';
	std::cout << std::left << std::setw(70) << "CONTAINER ID" << std::left << std::setw(40) << "NAME"
			  << std::left << std::setw(40) << "STATUS" << std::left << std::setw(40) << "IMAGE" << '\n';
	for(auto &c : containers) {
		std::cout << std::left << std::setw(70) << c.id << std::left << std::setw(40) << c.names[0]
				  << std::left << std::setw(40) << c.status << std::left << std::setw(40) << c.image << '\n';
	}
	std::cout << "--------------------" << '\n';

	return EXIT_SUCCESS;
}

