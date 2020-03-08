#ifndef _DOCKER_TYPES_H
#define _DOCKER_TYPES_H

#include <string>
#include <vector>

namespace docker_cpp
{
    struct ContainerInfo {
        std::string name;
    };

    using containerList = std::vector<ContainerInfo>;
} // namespace docker_cpp

#endif // _DOCKER_TYPES_H