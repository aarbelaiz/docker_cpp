#include "docker_cpp/docker.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace docker_cpp;

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

    // Lets create an image by pulling from dockerhub
    // DockerError err = docker.imageCreate("python", "", "", "3-alpine", "");
    // if (err.isError())
    // {
    //     std::cout << "Error creating image images\n";
    //     std::cout << err;
    //     return EXIT_FAILURE;
    // }
    // else
    // {
    //     std::cout << "Image successfully created...\n";
    // }

    ImageBuildParameters defaultParams;
    defaultParams.t = "foo:bar";
    DockerError err = docker.imageBuild(std::string("."), defaultParams);
    if (err.isError())
    {
        std::cout << "Error building image\n";
        std::cout << err;
        return EXIT_FAILURE;
    }

    // Get the id of the pulled image
    ImageList images;
    err = docker.imageList(images, true);
    if (err.isError())
    {
        std::cout << "Error listing images\n";
        return EXIT_FAILURE;
    }

    std::string imageId;
    for (ImageInfo &img : images)
    {
        bool containsTag = std::find(img.repoTags.begin(), img.repoTags.end(), "foo:bar") != img.repoTags.end();
        if (containsTag)
        {
            imageId = img.id;
            std::cout << imageId << '\n';
        }
    }

    // Lets tag the image
    err = docker.imageTag(imageId, "foo", "tag_bar");
    if (err.isError())
    {
        std::cout << "Error setting tag to image\n"
                  << err << '\n';
        return EXIT_FAILURE;
    }

    // Create a new container
    ContainterCreateResult createResult;
	ContainerCreateParams params;
	params.config.hostname = "localhost";
	params.config.image = imageId;
	params.config.user = "";
    //params.config.cmd = { "python3 -c \\\"print('Hello world!')\\\"" };
    params.config.cmd = { "python3 --version" };
    params.config.argsEscaped = true;
	//params.config.entrypoint = "/tmp";
	err = docker.containerCreate("dummy_container", params, createResult);
	if (err.isError()) {
		std::cout << err << '\n';
		return EXIT_FAILURE;
	}else{
        std::cout << createResult.id << std::endl;
    }

    err = docker.containerStart(createResult.id);
    if (err.isError()) std::cout << err << '\n';

    // Lets remove the image
    DeletedImageList deletedImages;
    err = docker.imageRemove(imageId, deletedImages, true, false);
    if (err.isError())
    {
        std::cout << "Error removing image\n"
                  << err << '\n';
        return EXIT_FAILURE;
    }
    else
    {
        for (DeletedImageInfo &info : deletedImages)
        {
            if (!info.deleted.empty())
                std::cout << "Deleted: " << info.deleted << '\n';
            if (!info.untagged.empty())
                std::cout << "Untagged: " << info.untagged << '\n';
        }
    }

    return EXIT_SUCCESS;
}