#ifndef _DOCKER_H
#define _DOCKER_H

#include "docker_types.h"
#include "docker_error.h"
#include "docker_http.h"
#include "docker_parse.h"

#include <string>

#include <asl/JSON.h>

#include <type_traits>

namespace asl
{
	class HttpResponse;
}

namespace docker_cpp
{
	template <typename T>
	class DOCKER_CPP_API Docker
	{
		static_assert(std::is_base_of<DockerHttpInterface<T>, T>::value, "T must derive from DockerHttpInterface");

	public:
		Docker(const std::string &uri)
		{
			_endpoint = uri + "/v1.40";
		}

		Docker(const std::string &ip, const unsigned int port)
		{
			Docker("http://" + ip + ":" + std::to_string(port), _server);
		}

		~Docker() = default;

		//////////// System

		/**
		 * Returns the version of Docker that is running and various information about the system that Docker is running on.
		 * @param [in,out] result Information (VersioInfo) about docker and the system.
		 * @returns DockerError
		 */
		DockerError version(VersionInfo &result)
		{
			const std::string url = _endpoint + "/version/json";
			auto res = _server.get(url);
			DockerError err = _checkError(res);
			if (!err.isOk())
				return err;
			auto data = res.json();
			parseVersionInfo(&data, result);
			return err;
		}

		/**
		 * Dummy endpoint to check connection with API Engine
		 */
		DockerError ping()
		{
			const std::string url = _endpoint + "/_ping";
			auto res = _server.get(url);
			return _checkError(res);
		}

		//////////// Images

		/**
		 * Returns a list of images on the server.
		 * @param [in,out] result A list of information (imageInfo) of each image in the server
		 * @param [in] all Show all images. Only images from a final layer (no children) are shown by default (default: false)
		 * @param [in] digests Show digest information as a RepoDigests field on each image (default: false)
		 * @returns DockerError
		 */
		DockerError images(ImageList &result, bool all = false, bool digests = false)
		{
			std::string url = _endpoint + "/images/json";
			url += query_params(q_arg("all", all), q_arg("digests", digests));
			auto res = _server.get(url);
			DockerError err = _checkError(res);
			if (!err.isOk())
				return err;
			parseImages(res.json(), result);
			return err;
		}
		//DockerError buildImage(const std::string &id);

		/**
		 * Create an image by either pulling it from a registry or importing it.
		 * @param [in] fromImage Name of the image to pull. The name may include a tag or digest. This parameter may only be used when pulling an image. The pull is cancelled if the HTTP connection is closed.
		 * @param [in] fromSrc Source to import. The value may be a URL from which the image can be retrieved or `-` to read the image from the request body. This parameter may only be used when importing an image.
		 * @param [in] repo Repository name given to an image when it is imported. The repo may include a tag. This parameter may only be used when importing an image.
		 * @param [in] tag Set commit message for imported image.
		 * @param [in] message Set commit message for imported image.
		 * @param [in] platform Platform in the format os[/arch[/variant]] (default: "").
		 * @returns DockerError
		 */
		DockerError createImage(const std::string &fromImage, const std::string &fromSrc, const std::string &repo, const std::string &tag, const std::string &message, const std::string &platform = "")
		{
			std::string url = _endpoint + "/images/create/json";
			url += query_params(q_arg("fromImage", fromImage));
			auto res = _server.get(url);
			return _checkError(res);
		}

		/**
		 * Tag an image so that it becomes part of a repository.
		 * @param [in] name Image name or ID to tag
		 * @param [in] repo The repository to tag in. E.g. "someuser/someimage"
		 * @param [in] tag The name of the new tag
		 * @returns DockerError
		 */
		DockerError tagImage(const std::string &name, const std::string &repo, const std::string &tag)
		{
			std::string url = _endpoint + "/images/" + name + "/tag/json";
			url += query_params(q_arg("repo", repo), q_arg("tag", tag));
			auto res = _server.post(url, "");
			return _checkError(res);
		}

		/**
		 * Remove an image, along with any untagged parent images that were referenced by that image.
		 * Images can't be removed if they have descendant images, are being used by a running container or are being used by a build.
		 * @param [in] name Image name or ID to tag
		 * @param [in] force Remove the image even if it is being used by stopped containers or has other tags (default: false)
		 * @param [in] noprune Do not delete untagged parent images (default: false)
		 */
		DockerError removeImage(const std::string &name, bool force = false, bool noprune = false)
		{
			std::string url = _endpoint + "/images/" + name + "/json";
			url += query_params(q_arg("force", force), q_arg("noprune", noprune));
			auto res = _server.delet(url);
			return _checkError(res);
		}

		//////////// Containers

		/**
		 * Returns a list of containers.
		 * @param [in,out] result A list of information (containerInfo) of each container in the server
		 * @param [in] all Return all containers. By default, only running containers are shown (default: false)
		 * @param [in] limit Return this number of most recently created containers, including non-running ones
		 * @param [in] size Return the size of container as fields SizeRw and SizeRootFs. (default: false)
		 * @returns DockerError
		 */
		DockerError containers(ContainerList &result, bool all = false, int limit = -1, bool size = false)
		{
			std::string url = _endpoint + "/containers/json";
			url += query_params(q_arg("all", all), q_arg("limit", limit), q_arg("size", size));
			auto res = _server.get(url);
			DockerError err = _checkError(res);
			if (!err.isOk())
				return err;
			asl::String filteredResponse = res.text().replace("\\\"", ""); // AAA: scaping is necessary for commands
			asl::Var data = asl::Json::decode(filteredResponse);
			parseContainers(&data, result);
			return err;
		}
		//DockerError createContainer(const std::string &name);

		/**
		 * Start a container.
		 * @param [in] id ID or name of the container
		 * @param [in] detachKeys Override the key sequence for detaching a container. Format is a single character [a-Z] or ctrl-<value> where <value> is one of: a-z, @, ^, [, , or _.
		 * @returns DockerError
		 */
		DockerError startContainer(const std::string &id, const std::string &detachKeys = "ctrl-c")
		{
			const std::string url = _endpoint + "/containers/" + id + "/start/json?detachKeys=" + detachKeys;
			auto res = _server.post(url, "");
			return _checkError(res);
		}

		/**
		 * Stop a container.
		 * @param [in] id ID or name of the container
		 * @param [in] t Number of seconds to wait before killing the container
		 * @returns DockerError
		 */
		DockerError stopContainer(const std::string &id, int t = -1)
		{
			std::string url = _endpoint + "/containers/" + id + "/stop/json";
			url += query_params(q_arg("t", t));
			auto res = _server.post(url, "");
			return _checkError(res);
		}

		/**
		 * Restart a container.
		 * @param [in] id ID or name of the container
		 * @param [in] t Number of seconds to wait before killing the container
		 * @returns DockerError
		 */
		DockerError restartContainer(const std::string &id, int t = -1)
		{
			std::string url = _endpoint + "/containers/" + id + "/restart/json";
			url += query_params(q_arg("t", t));
			auto res = _server.post(url, "");
			return _checkError(res);
		}

		/**
		 * Kill a container.
		 * @param [in] id ID or name of the container
		 * @param [in] signal Signal to send to the container as an integer or string e.g. SIGINT (defauult: SIGKILL)
		 * @returns DockerError
		 */
		DockerError killContainer(const std::string &id, const std::string &signal = "SIGKILL")
		{
			const std::string url = _endpoint + "/containers/" + id + "/kill/json?signal=" + signal;
			auto res = _server.post(url, "");
			return _checkError(res);
		}

		/**
		 * Rename a container.
		 * @param [in] id ID or name of the container
		 * @param [in] name New name for the container
		 * @returns DockerError
		 */
		DockerError renameContainer(const std::string &id, const std::string &name)
		{
			std::string url = _endpoint + "/containers/" + id + "/rename/json";
			url += query_params(q_arg("name", name));
			auto res = _server.post(url, "");
			return _checkError(res);
		}

		/**
		 * Block until a container stops, then returns the exit code.
		 * @param [in] id ID or name of the container
		 * @param [in,out] result Result with the exit code of the container
		 * @param [in] condition Wait until a container state reaches the given condition, either 'not-running' (default), 'next-exit', or 'removed'.
		 * @returns DockerError
		 */
		DockerError waitContainer(const std::string &id, WaitInfo &result, const std::string &condition = "not-running")
		{
			const std::string url = _endpoint + "/containers/" + id + "/wait/json?condition=" + condition;
			auto res = _server.post(url, "");
			DockerError err = _checkError(res);
			if (err.isError())
				return err;
			auto data = res.json();
			parseWaitInfo(&data, result);
			return err;
		}

		/**
		 * Remove a container.
		 * @param [in] id ID or name of the container
		 * @param [in] v Remove the volumes associated with the container (default: false)
		 * @param [in] force If the container is running, kill it before removing it (default: false)
		 * @param [in] link If the container is running, kill it before removing it (default: false)
		 * @returns DockerError
		 */
		DockerError removeContainer(const std::string &id, bool v = false, bool force = false, bool link = false)
		{
			std::string url = _endpoint + "/containers/" + id + "/json";
			url += query_params(q_arg("v", v), q_arg("force", force), q_arg("link", link));
			auto res = _server->delet(url);
			return _checkError(res);
		}

		////////// Exec

		/**
		 * Run a command inside a running container.
		 * @param [in] id ID name of the container
		 * @param [in] config Configuration parameters to execute a command
		 * @param [in,out] execId The id of the newly created instance
		 * @returns DockerError
		 */
		DockerError createExecInstance(const std::string &id, const ExecConfig &config, std::string &execId)
		{
			const std::string url = _endpoint + "/containers/" + id + "/exec";
			// TODO: create body
			auto res = _server->post(url, "body");
			DockerError err = _checkError(res);
			if (err.isError())
				return err;
			auto data = res.json();
			execId = *data["Id"].toString();
			return err;
		}

		/**
		 * Starts a previously set up exec instance.
		 * If detach is true, this endpoint returns immediately after starting the command.
		 * Otherwise, it sets up an interactive session with the command.
		 * @param [in] id Exec instance ID
		 * @param [in] detach Detach from the command
		 * @param [in] tty Allocate a pseudo-TTY
		 * @returns DockerError
		 */
		DockerError startExecInstance(const std::string &id, bool detach = true, bool tty = false)
		{
			const std::string url = _endpoint + "/exec/" + id + "/start";
			std::stringstream ss;
			ss << std::boolalpha << "{\"detach\":\"" << detach << "\",\"tty\":" << tty << "\"}";
			std::string body = ss.str();
			auto res = _server->post(url, body.c_str());
			return _checkError(res);
		}

		/**
		 * Resize the TTY session used by an exec instance.
		 * This endpoint only works if tty was specified as part of creating and starting the exec instance
		 * @param [in] id Exec instance ID
		 * @param [in] h Height of the TTY session in characters
		 * @param [in] w Width of the TTY session in characters
		 * @returns DockerError
		 */
		DockerError resizeExecInstance(const std::string &id, int h, int w)
		{
			std::string url = _endpoint + "/exec/" + id + "/resize";
			url += query_params(q_arg("h", h), q_arg("w", w));
			auto res = _server->post(url, "");
			return _checkError(res);
		}

		/**
		 * Return low-level information about an exec instance.
		 * @param [in] id Exec instance ID
		 * @param [in,out] result Information about the exec instance
		 * @returns DockerError
		 */
		DockerError inspectInstance(const std::string &id, ExecInfo &result)
		{
			const std::string url = _endpoint + "/exec/" + id + "/json";
			auto res = _server->get(url);
			DockerError err = _checkError(res);
			if (err.isError())
				return err;
			asl::Var data = asl::Json::decode(res.text().replace("\\\"", "")); // AAA: scaping is necessary for commands
			parseExec(&data, result);
			return err;
		}

		////////// Helper functions

		bool checkConnection() { return this->ping().isOk(); }

	private:
		std::string _endpoint;
		T _server;

		DockerError _checkError(const asl::HttpResponse &res)
		{
			int code = res.code();
			if (code == 200 || code == 204)
			{
				return DockerError::OK();
			}
			else if (code < 400)
			{
				std::string msg = *(res.json()["message"].toString());
				return DockerError::INFO(msg, code);
			}
			else
			{
				std::string msg = *(res.json()["message"].toString());
				return DockerError::ERROR(msg, code);
			}
		}
	};
} // namespace docker_cpp

#endif
