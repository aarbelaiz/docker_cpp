#include "docker_cpp/docker.h"

#include <iostream>

int main()
{
	docker_cpp::Docker docker("http://127.0.0.1:2375");
	docker_cpp::imageList res;
	docker.images(res);
	std::cout << res[0].id << std::endl;
	std::cout << res[0].sharedSize << std::endl;

	docker_cpp::containerList containers;
	docker.containers(containers, true);
	for(auto &c : containers) {
		std::cout << c.id << std::endl;
		std::cout << c.names[0] << std::endl;
	}
}

