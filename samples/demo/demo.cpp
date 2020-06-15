#include "docker_cpp/docker.h"

#include <iostream>

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
		std::cout << res[0].id << '\n';
		std::cout << res[0].sharedSize << '\n';
	}

	// List containers
	ContainerList containers;
	err = docker.containers(containers, true);
	if (err.isError()) {
		std::cout << err << '\n';
		return EXIT_FAILURE;
	}
	for(auto &c : containers) {
		std::cout << c.id << '\n';
		std::cout << c.names[0] << '\n';
	}

	return EXIT_SUCCESS;
}

