#ifndef _DOCKER_H
#define _DOCKER_H

#include "docker_types.h"
#include "docker_error.h"

#include <string>

namespace asl {
	class Var;
	class HttpResponse;
}

namespace docker_cpp
{
	class DOCKER_CPP_API Docker
	{
	public:
		Docker(const std::string &uri);
		Docker(const std::string &ip, const unsigned int port);
		~Docker(){};

		//////////// System

		/**
		 * Returns the version of Docker that is running and various information about the system that Docker is running on.
		 * @param [in,out] result Information (VersioInfo) about docker and the system.
		 * @returns DockerError
		 */
		DockerError version(VersionInfo &result);

		/**
		 * Dummy endpoint ot check connection.
		 */
		DockerError ping();

		//////////// Images

		/**
		 * Returns a list of images on the server.
		 * @param [in,out] result A list of information (imageInfo) of each image in the server.
		 * @param [in] all Show all images. Only images from a final layer (no children) are shown by default. (default: false)
		 * @param [in] digests Show digest information as a RepoDigests field on each image. (default: false)
		 * @returns DockerError
		 */
		DockerError images(imageList &result, bool all = false, bool digests=false);
		//DockerError buildImage(const std::string &id);
		
		/**
		 * 
		 */
		DockerError tagImage(const std::string &name, const std::string &repo, const std::string &tag);
		//DockerError removeImage(const std::string &name, bool force = false, bool noprune = false);

		//////////// Containers
		
		/**
		 * Returns a list of containers.
		 * @param [in,out] result A list of information (containerInfo) of each container in the server
		 * @param [in] all Return all containers. By default, only running containers are shown (default: false)
		 * @param [in] limit Return this number of most recently created containers, including non-running ones
		 * @param [in] size Return the size of container as fields SizeRw and SizeRootFs. (default: false)
		 * @returns DockerError
		 */
		DockerError containers(containerList &result, bool all = false, int limit = -1, bool size = false);
		//DockerError createContainer(const std::string &name);

		/**
		 * Start a container.
		 * @param [in] id ID or name of the container
		 * @param [in] detachKeys Override the key sequence for detaching a container. Format is a single character [a-Z] or ctrl-<value> where <value> is one of: a-z, @, ^, [, , or _.
		 * @returns DockerError
		 */
		DockerError startContainer(const std::string &id, const std::string &detachKeys = "ctrl-c");
		
		/**
		 * Stop a container.
		 * @param [in] id ID or name of the container
		 * @param [in] t Number of seconds to wait before killing the container
		 * @returns DockerError
		 */
		DockerError stopContainer(const std::string &id, int t = -1);
		
		/**
		 * Restart a container.
		 * @param [in] id ID or name of the container
		 * @param [in] t Number of seconds to wait before killing the container
		 * @returns DockerError
		 */
		DockerError restartContainer(const std::string &id, int t = -1);
		
		/**
		 * Kill a container.
		 * @param [in] id ID or name of the container
		 * @param [in] signal Signal to send to the container as an integer or string e.g. SIGINT (defauult: SIGKILL)
		 * @return DockerError
		 */
		DockerError killContainer(const std::string &id, const std::string &signal = "SIGKILL");

		//DockerError renameContainer(const std::string &id, const std::string &name);
		//DockerError waitContainer(const std::string &id, const std::string &condition = "not-running");
		//DockerError removeContainer(const std::string &id, bool v = false, bool force = false, bool link = false);

		////////// Exec

		//Run a command inside a running container.
		//DockerError createExecInstance(const std::string &id);
		//DockerError startExecInstance(const std::string &id, bool detach = true, bool tty = false);
		//DockerError inspectInstance(const std::string &id);

		////////// Helper functions

		bool checkConnection();

	private:
		std::string _endpoint;

		DockerError _checkError(asl::HttpResponse *res);
	};
}

#endif
