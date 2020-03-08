#ifndef _DOCKER_H
#define _DOCKER_H

#include "docker_types.h"
#include "export.h"

namespace docker_cpp
{
	class DOCKER_CPP_API Docker
	{
	public:
		Docker(const std::string &uri);
		~Docker(){};

		// Connection

		// Images

		// Containers

		// Prcoess
	private:
		int _x;

		void requestAndParse();
	};
}

#endif
