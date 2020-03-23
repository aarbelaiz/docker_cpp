#ifndef _DOCKER_H
#define _DOCKER_H

#include "docker_types.h"
#include "docker_error.h"

#include <string>

namespace asl {
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
		 * Dummy endpoint to check connection with API Engine
		 */
		DockerError ping();

		//////////// Images

		/**
		 * Returns a list of images on the server.
		 * @param [in,out] result A list of information (imageInfo) of each image in the server
		 * @param [in] all Show all images. Only images from a final layer (no children) are shown by default (default: false)
		 * @param [in] digests Show digest information as a RepoDigests field on each image (default: false)
		 * @returns DockerError
		 */
		DockerError images(imageList &result, bool all = false, bool digests=false);
		//DockerError buildImage(const std::string &id);
		
		/**
		 * Tag an image so that it becomes part of a repository.
		 * @param [in] name Image name or ID to tag
		 * @param [in] repo The repository to tag in. E.g. "someuser/someimage"
		 * @param [in] tag The name of the new tag
		 * @returns DockerError
		 */
		DockerError tagImage(const std::string &name, const std::string &repo, const std::string &tag);

		/**
		 * Remove an image, along with any untagged parent images that were referenced by that image.
		 * Images can't be removed if they have descendant images, are being used by a running container or are being used by a build.
		 * @param [in] name Image name or ID to tag
		 * @param [in] force Remove the image even if it is being used by stopped containers or has other tags (default: false)
		 * @param [in] noprune Do not delete untagged parent images (default: false)
		 */
		DockerError removeImage(const std::string &name, bool force = false, bool noprune = false);

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
		 * @returns DockerError
		 */
		DockerError killContainer(const std::string &id, const std::string &signal = "SIGKILL");

		/**
		 * Rename a container.
		 * @param [in] id ID or name of the container
		 * @param [in] name New name for the container
		 * @returns DockerError
		 */
		DockerError renameContainer(const std::string &id, const std::string &name);
		
		/**
		 * Block until a container stops, then returns the exit code.
		 * @param [in] id ID or name of the container
		 * @param [in] condition Wait until a container state reaches the given condition, either 'not-running' (default), 'next-exit', or 'removed'.
		 * @returns DockerError
		 */
		DockerError waitContainer(const std::string &id, const std::string &condition = "not-running");
		
		/**
		 * Remove a container.
		 * @param [in] id ID or name of the container
		 * @param [in] v Remove the volumes associated with the container (default: false)
		 * @param [in] force If the container is running, kill it before removing it (default: false)
		 * @param [in] link If the container is running, kill it before removing it (default: false)
		 * @returns DockerError
		 */
		DockerError removeContainer(const std::string &id, bool v = false, bool force = false, bool link = false);

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
