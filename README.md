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

For example, to get the collection of the docker images:

```c++

 // List images
 ImageList images;
 DockerError err = docker.images(images);
 if (err.isOk()) {
     // images will contain an array of image data
 }else{
     std::cerr << "ERROR: List of image could not be retrieved!" << std::endl;
 }
```

For more examples, see the `samples` directory.

## Dependencies

- [ASL](https://github.com/aslze/asl) - All-purpose Simple Library

## Docker Engine API

Developed with version [v1.40](https://docs.docker.com/engine/api/v1.40/) of the docker API engine.

### API coverage

| API Feature                | Supported |
| :--------------------------- | --------: |
| __Authentication__           | :x: |
| __System__                   |     |
| Check auth config            | :x: |
| Get system information       | :x: |
| Get version                  | :heavy_check_mark: |
| Ping                         | :heavy_check_mark: |
| Monitor events               | :x: |
| Get data usage information   | :x: |
| __Images__                   | -  |
| List images                  | :clock9: |
| Build an image               | :clock9: |
| Delete builder cache         | :x: |
| Create an image              | :clock9: |
| Inspect an image             | :x: |
| Get history of an image      | :x: |
| Push an image                | :x: |
| Tag an image                 | :clock9: |
| Remove an image              | :clock9: |
| Search images                | :x: |
| Delete unused images         | :clock9: |
| Create a new image from cont.| :x: |
| Export an image              | :x: |
| Export several images        | :x: |
| Import images                | :x: |
| __Containers__     | :x:  |
| __Networks__       | :x:  |
| __Volumes__        | :x:  |
| __Exec__           | :x:  |
| __Swarm__          | :x:  |
| __Nodes__          | :x:  |
| __Services__       | :x:  |
| __Tasks__          | :x:  |
| __Secrets__        | :x:  |
| __Configs__        | :x:  |
| __Plugins__        | :x:  |
| __Distribution__   | :x:  |
| __Distribution__   | :x:  |
