#ifndef _DOCKER_PARSE_H
#define _DOCKER_PARSE_H

#include "docker_types.h"

namespace asl {
    class Var;
}

namespace docker_cpp
{
    void parseImages(asl::Var *in, imageList &out);
    void parseContainers(asl::Var *in, containerList &out);
    void parsePort(asl::Var *in, Port &out);
    void parseNetwork(asl::Var *in, NetworkSettings &out); 
} // namespace docker_cpp


#endif //_DOCKER_PARSE_H