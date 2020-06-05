#include "docker_cpp/docker.h"

#include <iostream>

using namespace docker_cpp;

int main()
{
	Docker docker("http://127.0.0.1:2375");

	// Check if there is a connection with docker daemon
	bool connected = docker.checkConnection();
	if (!connected) {
		std::cout << "Could not connect to docker engine!" << std::endl;
		return EXIT_FAILURE;
	}

	// List images
	ImageList res;
	DockerError err = docker.images(res);
	if (err.isOk()) {
		std::cout << res[0].id << std::endl;
		std::cout << res[0].sharedSize << std::endl;
	}

	// List containers
	ContainerList containers;
	err = docker.containers(containers, true);
	if (err.isError()) {
		std::cout << err << std::endl;
		return EXIT_FAILURE;
	}
	for(auto &c : containers) {
		std::cout << c.id << std::endl;
		std::cout << c.names[0] << std::endl;
	}

	return EXIT_SUCCESS;
}

