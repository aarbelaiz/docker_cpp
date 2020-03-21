#ifndef _DOCKER_H
#define _DOCKER_H

#include "docker_types.h"
#include "docker_error.h"

#include <string>

namespace asl {
	class Var;
}

namespace docker_cpp
{
	class DOCKER_CPP_API Docker
	{
	public:
		Docker(const std::string &uri);
		Docker(const std::string &ip, const unsigned int port);

		~Docker(){};

		//////////// Connection
		DockerError info();
		// Returns the used version for the Docker API
		std::string version() { return "v1.40"; };

		//////////// Images

		/**
		 * Returns a list of images on the server.
		 * @param [in,out] result A list of information (imageInfo) of each image in the server.
		 * @param [in] all Show all images. Only images from a final layer (no children) are shown by default. (default: false)
		 * @param [in] digests Show digest information as a RepoDigests field on each image. (default: false)
		 */
		DockerError images(imageList &result, bool all = false, bool digests=false);
		//dockErr buildImage(const std::string &id);

		//////////// Containers
		
		/**
		 * Returns a list of containers.
		 * @param result A list of information (containerInfo) of each container in the server.
		 * @param all Return all containers. By default, only running containers are shown (default: false).
		 * @param limit Return this number of most recently created containers, including non-running ones.
		 * @param size Return the size of container as fields SizeRw and SizeRootFs. (default: false)
		 */
		DockerError containers(containerList &result, bool all = false, int limit = -1, bool size = false);
		
		/**
		 * Start a container.
		 * @param id ID or name of the container
		 * @param detachKeys Override the key sequence for detaching a container. Format is a single character [a-Z] or ctrl-<value> where <value> is one of: a-z, @, ^, [, , or _.
		 * @returns dockErr
		 */
		DockerError runContainer(const std::string &id, const std::string &detachKeys = "ctrl-c");
		DockerError stopContainer(const std::string &id, int t = -1);
		DockerError restartContainer(const std::string &id, int t = -1);
		/**
		 * Kill a container.
		 * @param id ID or name of the container
		 * @param signal Signal to send to the container as an integer or string (e.g. SIGINT)
		 * @return dockErr
		 */
		DockerError killContainer(const std::string &id, const std::string &signal = "SIGKILL");

		//void listContainers();
		//void runContainer();
		//void killContainer();
		//void stopContainer();

		// Process
		//void restart();
		//void stop();

		//Helper functions

	private:
		std::string _endpoint;

		void _parseImages(asl::Var *in, imageList &out);
		void _parseContainers(asl::Var *in, containerList &out);
		void _parsePort(asl::Var *in, Port &out);
		void _parseNetwork(asl::Var *in, NetworkSettings &out);
	};
}

#endif
