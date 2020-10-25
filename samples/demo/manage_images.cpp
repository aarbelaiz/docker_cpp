#include "docker_cpp/docker.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace docker_cpp;

template<class T>
bool findImage(Docker<T> &d, const std::string& tag, ImageInfo& image)
{
    ImageList images;
    DockerError err = d.imageList(images, true);
    if (err.isError()) return false;

    bool containsTag = false;
    for (ImageInfo &img : images)
    {
        bool containsTag = std::find(img.repoTags.begin(), img.repoTags.end(), tag) != img.repoTags.end();
        if (containsTag)
        {
            image = img;
            break;
        }
    }
    return containsTag;
}

int main()
{
    Docker<ASLHttp> docker("http://127.0.0.1:2375");

    // Check if there is a connection with docker daemon
    bool connected = docker.checkConnection();
    if (!connected)
    {
        std::cout << "Could not connect to docker engine!\n";
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "Connection successfully established...\n";
    }

    // Check if the image already exists
    const std::string IMAGE_TAG = "foo:bar";

    ImageInfo img;
    bool exists = findImage(docker, IMAGE_TAG, img);
    
    bool tagNewImage = false;
    if (!exists) {
        //First, try to build an image from a Dockerfile
        ImageBuildParameters defaultParams;
        defaultParams.t = IMAGE_TAG;
        DockerError err = docker.imageBuild(std::string("."), defaultParams);
        if (err.isError())
        {
            std::cout << "Error building image\n";
            std::cout << err;
            
            // Then, try to download from dockerhub
            DockerError err = docker.imageCreate("python", "", "", "3-alpine,foo:bar", "");
            if (err.isError())
            {
                std::cout << "Error creating image images\n";
                std::cout << err;
                return EXIT_FAILURE;
            }
            else
            {
                std::cout << "Image successfully created...\n";
                tagNewImage = true;
            }
        }

    }

    findImage(docker, IMAGE_TAG, img);

    if (tagNewImage) {
        // Lets tag the image
        DockerError err = docker.imageTag(img.id, "foo", "bar");
        if (err.isError())
        {
            std::cout << "Error setting tag to image\n" << err << '\n';
            return EXIT_FAILURE;
        }
    }   

    // Create a new container
    ContainterCreateResult createResult;
	ContainerCreateParams params;
	params.config.hostname = "localhost";
	params.config.image = img.id.substr(7);
	params.config.user = "";
    //params.config.cmd = { "python3 -c \\\"print('Hello world!')\\\"" };
    params.config.cmd = { "python3", "--version" };
    //params.config.cmd = { "echo", "'hello'" };
    params.config.argsEscaped = false;
    params.config.exposedPorts = { "80/tcp" };
	params.config.entrypoint = { "" }; // Reset
	DockerError err = docker.containerCreate("dummy_container", params, createResult);
	if (err.isError()) {
		std::cout << err << '\n';
		return EXIT_FAILURE;
	}else{
        std::cout << "[Container created]: " << createResult.id << std::endl;
    }

    err = docker.containerStart(createResult.id);
    if (err.isError()) std::cout << "Error starting container...\n" << err << '\n';

    WaitInfo info;
    err = docker.containerWait(createResult.id, info);
    if (err.isError()) std::cout << "Error waiting for container...\n" << err << '\n';

    // Lets remove the image
    // DeletedImageList deletedImages;
    // err = docker.imageRemove(img.id, deletedImages, true, false);
    // if (err.isError())
    // {
    //     std::cout << "Error removing image\n"
    //               << err << '\n';
    //     return EXIT_FAILURE;
    // }
    // else
    // {
    //     for (DeletedImageInfo &info : deletedImages)
    //     {
    //         if (!info.deleted.empty())
    //             std::cout << "Deleted: " << info.deleted << '\n';
    //         if (!info.untagged.empty())
    //             std::cout << "Untagged: " << info.untagged << '\n';
    //     }
    // }

    // Remove the create container
    err = docker.containerRemove(createResult.id, false, true, false);
    if (err.isError())
    {
        std::cout << "Error removing image\n"
                  << err << '\n';
        return EXIT_FAILURE;
    }
    else
    {
        std::cout << "Removed: " << createResult.id << std::endl;
    }

    return EXIT_SUCCESS;
}