#ifndef _DOCKER_PARSE_H
#define _DOCKER_PARSE_H

#include "docker_types.h"

namespace asl {
    class Var;
}

namespace docker_cpp
{
    void parseImages(const asl::Var &in, ImageList &out);
    void parseContainers(asl::Var *in, ContainerList &out);
    void parsePort(asl::Var *in, Port &out);
    void parseNetwork(asl::Var *in, NetworkSettings &out);
    void parseVersionInfo(asl::Var *in, VersionInfo &out);
    void parseWaitInfo(asl::Var *in, WaitInfo &out);
    void parseExec(asl::Var *in, ExecInfo &out);
} // namespace docker_cpp


#endif //_DOCKER_PARSE_H