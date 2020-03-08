#include <docker_cpp/docker.h>

#include <iostream>

namespace docker_cpp {

Docker::Docker(const std::string &uri)
{
	std::cout << uri << std::endl;
	_x = 0;
}

}
