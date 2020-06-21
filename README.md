# docker_cpp

*WIP*: `WARNING:` Do not expect full API support.

A simple C++ client to comunicate with a local or remote Docker server.

## Usage

```c++
#include <docker_cpp/docker.h>

using namespace docker_cpp;

Docker<ASLHttp> docker("http://127.0.0.1:2375");
```

There is a helper function to check if a connection with the docker server can be established.

```c++
bool ok = docker.checkConnection();
if (ok) {
    // The client can communicate with the docker engine.
    ...
}
```

For example, to get the collection of images in the docker server:

```c++
 ImageList images;
 DockerError err = docker.imageList(images);
 if (err.isOk()) {
     // images is a vector populated with information about each docker image
     for (ImageInfo &im : images) { // To print the id of each image
         std::cout << "ID: " << im.id << '\n';
     }
 }else{
     std::cerr << "ERROR: List of image could not be retrieved!" << '\n';
 }
```

For more examples, see the `samples` directory an also check the API coverage.

## Dependencies

- [ASL](https://github.com/aslze/asl) - All-purpose Simple Library

## Docker Engine API

Developed with version [v1.40](https://docs.docker.com/engine/api/v1.40/) of the docker API engine.

### API coverage

(:100:: Tested, :heavy_check_mark:: It may work, :clock9:: planned, :x:: Not supported)

| API Feature                  | Supported |
| :--------------------------- | --------: |
| __Authentication__           | :x: |
| __System__                   |     |
| Check auth config            | :x: |
| Get system information       | :x: |
| Get version                  | :heavy_check_mark: |
| Ping                         | :heavy_check_mark: |
| Monitor events               | :x: |
| Get data usage information   | :x: |
| __Images__                   |     |
| List images                  | :heavy_check_mark: |
| Build an image               | :clock9: |
| Delete builder cache         | :x: |
| Create an image              | :clock9: |
| Inspect an image             | :x: |
| Get history of an image      | :x: |
| Push an image                | :x: |
| Tag an image                 | :heavy_check_mark: |
| Remove an image              | :heavy_check_mark: |
| Search images                | :x: |
| Delete unused images         | :heavy_check_mark: |
| Create a new image from cont.| :x: |
| Export an image              | :x: |
| Export several images        | :x: |
| Import images                | :x: |
| __Containers__               |     |
| List containers              | :clock9: |
| Create a container           | :clock9: |
| Inspect a container          | :x: |
| List processes running inside a container | :x: |
| Get container logs           | :x: |
| Get changes on a container's filesystem | :x: |
| Export a container           | :x: |
| Get container stats          | :x: |
| Resize a container TTY       | :x: |
| Start a container            | :clock9: |
| Restart a container          | :clock9: |
| Kill a container             | :clock9: |
| Update a container           | :x: |
| Rename a container           | :clock9: |
| Pause a container            | :clock9: |
| Unpause a container          | :clock9: |
| Attatch to a container       | :x: |
| Attatch to a container webs. | :x: |
| Wait for a container         | :x: |
| Remove a container           | :clock9: |
| Get information about files in container | :x: |
| Get an archive of a filesystem resource in a container | :x: |
| Extract an archive of files or folders to a directory in a container | :x: |
| Delete stopped containers    | :clock9: |
| __Networks__       | :x:  |
| __Volumes__        | :x:  |
| __Exec__           |      |
| Create an exec instance      | :heavy_check_mark: |
| Start an exec instance       | :heavy_check_mark: |
| Resize an exec instance      | :heavy_check_mark: |
| Inspect an exec instance     | :heavy_check_mark: |
| __Swarm__          | :x:  |
| __Nodes__          | :x:  |
| __Services__       | :x:  |
| __Tasks__          | :x:  |
| __Secrets__        | :x:  |
| __Configs__        | :x:  |
| __Plugins__        | :x:  |
| __Distribution__   | :x:  |
